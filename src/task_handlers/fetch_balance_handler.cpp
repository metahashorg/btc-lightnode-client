#include "fetch_balance_handler.h"
#include "exception/except.h"

// fetch_balance_handler
fetch_balance_handler::fetch_balance_handler(http_session_ptr& session)
    : base_network_handler(settings::server::address, session)
{
}

fetch_balance_handler::fetch_balance_handler()
    : base_network_handler(settings::server::address)
{
}

bool fetch_balance_handler::prepare_params()
{
    BGN_TRY
    {
        CHK_PRM(m_id, "id field not found")

        auto params = m_reader.get_params();
        CHK_PRM(params, "params field not found")

        std::string addr;
        CHK_PRM(m_reader.get_value(*params, "address", addr), "address field not found")
        CHK_PRM(!addr.empty(), "address empty")

        m_writer.add("method", "address.balance");
        m_writer.add("token", settings::service::token);

        params = m_writer.get_params();
        params->SetArray();

        rapidjson::Value obj(rapidjson::kObjectType);

        obj.AddMember("currency", rapidjson::Value().SetInt(settings::service::coin_key), m_writer.get_allocator());

        rapidjson::Value addr_arr(rapidjson::kArrayType);
        addr_arr.PushBack(rapidjson::Value().SetString(addr, m_writer.get_allocator()),
                          m_writer.get_allocator());

        obj.AddMember("address", addr_arr, m_writer.get_allocator());

        params->PushBack(obj, m_writer.get_allocator());

        return true;
    }
    END_TRY_RET(false)
}
