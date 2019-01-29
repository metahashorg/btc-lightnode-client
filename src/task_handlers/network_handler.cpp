#include "network_handler.h"
#include "http_json_rpc_request.h"
#include "http_session.h"

base_network_handler::base_network_handler(const std::string& host, http_session_ptr& session)
    : base_handler(session) {
    m_request = std::make_shared<http_json_rpc_request>(host, session->get_io_context());
}
base_network_handler::base_network_handler(const std::string& host) {
    m_request = std::make_shared<http_json_rpc_request>(host, m_io_ctx);
}

void base_network_handler::execute()
{
    BGN_TRY
    {
        m_request->set_path(m_reader.get_method());
        m_request->set_body(m_writer.stringify());

        m_result.pending = m_async_execute;
        if (!m_async_execute) {
            m_request->execute();
            m_writer.reset();
            m_writer.parse(m_request->get_result().c_str());
        } else {
            m_request->execute_async(boost::bind(&base_network_handler::on_complete, shared_from(this), m_id, m_request, m_session));
        }
    }
    END_TRY
}


void base_network_handler::on_complete(json_rpc_id id, http_json_rpc_request_ptr req, http_session_ptr session)
{
    BGN_TRY
    {
        json_rpc_reader reader;
        m_writer.reset();

        CHK_PRM(reader.parse(req->get_result().c_str()), "Invalid response json")

        json_rpc_id _id = reader.get_id();

        CHK_PRM(_id != 0 && _id == id, "Returned id doesn't match")

        auto err = reader.get_error();
        auto res = reader.get_result();

        CHK_PRM(err || res, "No occur result or error")

        if (err) {
            m_writer.set_error(*err);
        } else if (res) {
            m_writer.set_result(*res);
        }

        rapidjson::Document& doc = reader.get_doc();
        for (auto& m : doc.GetObject())
        {
            std::string name = m.name.GetString();
            std::transform(name.begin(), name.end(), name.begin(), ::tolower);
            if (std::find(json_rpc_service.begin(), json_rpc_service.end(), name) != json_rpc_service.end())
                continue;
            m_writer.add_value(m.name.GetString(), m.value);
        }

        boost::asio::post(boost::bind(&http_session::send_json, session, m_writer.stringify()));
    }
    END_TRY_PARAM(boost::asio::post(boost::bind(&http_session::send_json, session, m_writer.stringify())))
}
