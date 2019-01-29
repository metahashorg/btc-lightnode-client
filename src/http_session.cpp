#include "http_session.h"
#include "task_handlers/task_handlers.h"
#include "task_handlers/base_handler.h"
#include "json_rpc.h"
#include "log/log.h"

http_session::http_session(tcp::socket&& socket) :
    m_socket(std::move(socket))
{
}

http_session::~http_session()
{
    boost::system::error_code ec;
    m_socket.shutdown(m_socket.shutdown_both, ec);
    m_socket.close(ec);
}

void http_session::run()
{
    m_req.clear();
    m_req.body().clear();
    m_buf.consume(m_buf.size());

    auto self = shared_from_this();
    http::async_read(m_socket, m_buf, m_req, [self](beast::error_code ec, std::size_t bytes_transferred)
    {
        if (!ec && bytes_transferred > 0)
        {
            self->process_request();
            self->run();
        }
    });
}

asio::io_context& http_session::get_io_context()
{
    return m_socket.get_io_context();
}

void http_session::process_request()
{
    LOG_DBG("%s >> %s", m_socket.remote_endpoint().address().to_string().c_str(), m_req.body().data());

    if (m_req.target().to_string() != "/")
    {
        send_bad_request("Incorrect path");
        return;
    }

    if (m_req.method() != http::verb::post)
    {
        send_bad_request("Incorrect http method");
        return;
    }

    std::string json;
    json_rpc_reader reader;
    json_rpc_writer writer;
    if (reader.parse(m_req.body().c_str()))
    {
        auto it = map_handlers.find(reader.get_method());
        if (it == map_handlers.end())
        {
            LOG_DBG("Incorrect service method \"%s\"", reader.get_method().c_str())

            writer.set_id(reader.get_id());
            writer.set_error(-32601, "Method not found");
            json = writer.stringify();
        }
        else
        {
            auto ptr = shared_from_this();
            auto res = it->second(ptr, m_req.body());
            // async operation
            if (!res)
                return;
            json.append(res.message);
        }
    }
    else
    {
        LOG_DBG("Incorrect json %s", m_req.body().c_str())

        writer.set_error(-32700, "Parse error");
        json = writer.stringify();
    }

    send_json(json);
}

void http_session::send_bad_request(const char* error)
{
    http::response<http::dynamic_body> response;
    response.result(http::status::bad_request);
    response.set(http::field::content_type, "text/plain");
    beast::ostream(response.body()) << error;
    send_response(response);
}

void http_session::send_json(const std::string& data)
{
    http::response<http::dynamic_body> response;
    response.result(http::status::ok);
    response.set(http::field::content_type, "application/json");
    beast::ostream(response.body()) << data.c_str();
    send_response(response);
}

void http_session::send_response(http::response<http::dynamic_body>& response)
{
    LOG_DBG("%s << %s", m_socket.remote_endpoint().address().to_string().c_str(), beast::buffers_to_string(response.body().data()).c_str());

    response.version(10);
    response.set(http::field::server, "btc.service");
    response.set(http::field::content_length, response.body().size());
    response.set(http::field::keep_alive, true);
    response.keep_alive(true);
    http::write(m_socket, response);
}
