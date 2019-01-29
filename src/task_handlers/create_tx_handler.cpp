#include "create_tx_handler.h"

#include "boost/asio/post.hpp"
#include "boost/bind/bind.hpp"
#include "http_session.h"

void create_tx_handler::execute()
{
    // do nothing, just prepare
    m_writer.set_result(rapidjson::Value().SetString("ok"));

    boost::asio::post(boost::bind(&http_session::send_json, m_session, m_writer.stringify()));
    return;
}
