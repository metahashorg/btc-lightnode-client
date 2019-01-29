#include "fetch_history_handler.h"

// fetch_history_handler
bool fetch_history_handler::prepare_params()
{
    BGN_TRY
    {
        CHK_PRM(m_id, "id field not found")

        auto params = m_reader.get_params();
        CHK_PRM(params, "params field not found")

        std::string address;
        CHK_PRM(m_reader.get_value(*params, "address", address), "address field not found")
        CHK_PRM(!address.empty(), "address empty")

        m_writer.add("method", "address.transaction");
        m_writer.add("token", settings::service::token);

        params = m_writer.get_params();
        params->SetArray();

        rapidjson::Value obj(rapidjson::kObjectType);

        obj.AddMember("currency",
                      rapidjson::Value().SetInt(settings::service::coin_key),
                      m_writer.get_allocator());

        obj.AddMember("address",
                      rapidjson::Value().SetString(address, m_writer.get_allocator()),
                      m_writer.get_allocator());

        params->PushBack(obj, m_writer.get_allocator());

        return true;
    }
    END_TRY_RET(false)
}
