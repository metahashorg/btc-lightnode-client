#ifndef _NETWORK_HANDLER_H__
#define _NETWORK_HANDLER_H__

#include "base_handler.h"
#include "http_json_rpc_request_ptr.h"
#include "http_session_ptr.h"

#define BOOST_ERROR_CODE_HEADER_ONLY
#include <boost/asio/io_context.hpp>

class base_network_handler : public base_handler
{
public:
    base_network_handler(const std::string& host, http_session_ptr& session);
    base_network_handler(const std::string& host);

    virtual void execute() override;

protected:
    // async callback
    void on_complete(json_rpc_id id, http_json_rpc_request_ptr req, http_session_ptr session);

protected:
    http_json_rpc_request_ptr   m_request;
    std::string                 m_method;
    boost::asio::io_context     m_io_ctx;
    bool                        m_async_execute = {true};
};

#endif // _NETWORK_HANDLER_H__
