#include "create_tx_base_handler.h"

#include "boost/asio/post.hpp"
#include "boost/bind/bind.hpp"

#include "http_json_rpc_request.h"
#include "http_session.h"

create_tx_base_handler::create_tx_base_handler(http_session_ptr& session):
    base_network_handler(settings::server::address, session) {
}

create_tx_base_handler::~create_tx_base_handler() {
}

bool create_tx_base_handler::prepare_params()
{
    BGN_TRY
    {
        CHK_PRM(m_id, "id field not found")

        auto params = m_reader.get_params();
        CHK_PRM(params, "params field not found")

        CHK_PRM(m_reader.get_value(*params, "address", this->m_address), "address field not found")
        CHK_PRM(!m_address.empty(), "address field empty")

        CHK_PRM(m_reader.get_value(*params, "password", this->m_password), "password field not found")
        CHK_PRM(!m_password.empty(), "empty password")

        CHK_PRM(m_reader.get_value(*params, "to", this->m_to), "to field not found")
        CHK_PRM(!m_to.empty(), "to field empty")

        auto jValue = m_reader.get("value", *params);
        CHK_PRM(jValue, "value field not found")
        if (jValue->IsString())
        {
            m_value = jValue->GetString();
            std::transform(m_value.begin(), m_value.end(), m_value.begin(), ::tolower);
        }
        if (m_value.compare("all") != 0)
        {
            CHK_PRM(json_utils::val2hex(jValue, m_value), "value field incorrect format")
        }

        jValue = m_reader.get("nonce", *params);
        if (jValue)
        {
            CHK_PRM(json_utils::val2hex(jValue, m_nonce), "nonce field incorrect format")
        }

        jValue = m_reader.get("fee", *params);
        CHK_PRM(jValue, "fee field not found")
        if (jValue->IsString())
        {
            m_fee = jValue->GetString();
            std::transform(m_fee.begin(), m_fee.end(), m_fee.begin(), ::tolower);
        }
        if (m_fee.compare("auto") != 0)
        {
            CHK_PRM(json_utils::val2hex(jValue, m_fee), "fee field incorrect format")
        }

        m_btc_wallet = std::make_unique<BtcWallet>(settings::system::wallet_stotage, m_address, m_password);

        m_result.pending = true;
        CHK_PRM(get_transaction_params(), "get transaction params error")

        return false;
    }
    END_TRY_RET_PARAM(false, boost::asio::post(boost::bind(&http_session::send_json, m_session, m_writer.stringify())))
}

bool create_tx_base_handler::get_transaction_params()
{
    BGN_TRY
    {
        json_rpc_writer writer;
        json_rpc_id id = 1;
        writer.set_id(id);
        writer.add("method", "transaction.params");
        writer.add("token", settings::service::token);

        auto params = writer.get_params();
        params->SetArray();

        rapidjson::Value obj(rapidjson::kObjectType);

        rapidjson::Value cur(rapidjson::kNumberType);
        cur.SetInt(settings::service::coin_key);
        obj.AddMember("currency", cur, writer.get_allocator());

        rapidjson::Value addr(rapidjson::kStringType);
        addr.SetString(this->m_address, writer.get_allocator());
        obj.AddMember("address", addr, writer.get_allocator());

        params->PushBack(obj, writer.get_allocator());

        auto req = std::make_shared<http_json_rpc_request>(settings::server::address, this->m_session->get_io_context());
        req->set_body(writer.stringify());
        req->execute_async(boost::bind(&create_tx_base_handler::on_get_transaction_params, shared_from(this), req, id));

        return true;
    }
    END_TRY_RET(false)
}

void create_tx_base_handler::on_get_transaction_params(http_json_rpc_request_ptr req, json_rpc_id id)
{
    BGN_TRY
    {

        if (!check_json(req, id))
        {
            boost::asio::post(boost::bind(&http_session::send_json, m_session, m_writer.stringify()));
            return;
        }

        json_rpc_reader reader;
        CHK_PRM(reader.parse(req->get_result().c_str()), "invalid json")

        auto err = reader.get_error();
        CHK_PRM(err == nullptr, reader.stringify(err))

        rapidjson::Document& doc = reader.get_doc();
        auto data = doc.FindMember("data");
        CHK_PRM(data != doc.MemberEnd(), "data field not found")

        rapidjson::Value& data_val = data->value;
        CHK_PRM(data_val.IsObject(), "data field incorrect")

        if (m_nonce.empty())
        {
            auto vnonce = data_val.FindMember("nonce");
            CHK_PRM(vnonce != data_val.MemberEnd(), "nonce field not found")
            CHK_PRM(json_utils::val2hex(&vnonce->value, m_nonce), "nonce field incorrect format")
        }
        if (m_nonce.empty())
        {
            LOG_WRN("empty nonce")
        }

        auto vutxos = data_val.FindMember("utxos");
        CHK_PRM(vutxos != data_val.MemberEnd(), "remote service 'utxos' field not found")
        if (vutxos->value.IsArray())
        {
            for (auto& v: vutxos->value.GetArray())
            {
                if (!v.IsObject())
                    continue;

                BtcInput tx;
                auto tx_hash = v.FindMember("tx_hash");
                if (tx_hash == v.MemberEnd() || !tx_hash->value.IsString())
                    continue;
                tx.tx_hash = tx_hash->value.GetString();

                auto tx_index = v.FindMember("tx_index");
                if (tx_index == v.MemberEnd() || !tx_index->value.IsInt())
                    continue;
                tx.tx_index = tx_index->value.GetUint();

                auto val = v.FindMember("value");
                if (val == v.MemberEnd() || !val->value.IsString())
                    continue;
                std::istringstream iss(val->value.GetString());
                iss >> tx.value;

                auto script = v.FindMember("scriptPubKey");
                if (script == v.MemberEnd() || !script->value.IsString())
                    continue;
                tx.scriptPubkey = script->value.GetString();

                this->m_utxos.push_back(tx);
            }
        }

        m_estimate_fee.clear();
        auto vfee = data_val.FindMember("estimate_price_fee");
        CHK_PRM(vfee != data_val.MemberEnd(), "estimate_price_fee field not found")
        CHK_PRM(json_utils::val2hex(&vfee->value, m_estimate_fee), "estimate_price_fee field incorrect format")
        if (m_estimate_fee.empty())
        {
            LOG_WRN("empty fee")
        }

        build_request();
    }
    END_TRY_PARAM(boost::asio::post(boost::bind(&http_session::send_json, m_session, m_writer.stringify())))
}

void create_tx_base_handler::build_request()
{
    BGN_TRY
    {
        size_t estimateFeeInSatoshi(0);
        if (!m_estimate_fee.empty())
        {
            estimateFeeInSatoshi = std::stoul(m_estimate_fee);
        }
        auto res = m_btc_wallet->buildTransaction(m_utxos, estimateFeeInSatoshi, m_value, m_fee, m_to);

        m_writer.set_method("transaction.add.raw");
        m_writer.add("token", settings::service::token);
        auto params = m_writer.get_params();
        params->SetArray();

        rapidjson::Value obj(rapidjson::kObjectType);

        rapidjson::Value cur(rapidjson::kNumberType);
        cur.SetInt(settings::service::coin_key);
        obj.AddMember("currency", cur, m_writer.get_allocator());

        rapidjson::Value tx(rapidjson::kStringType);
        tx.SetString(res.first, m_writer.get_allocator());
        obj.AddMember("raw", tx, m_writer.get_allocator());

        params->PushBack(obj, m_writer.get_allocator());

        boost::asio::post(boost::bind(&create_tx_base_handler::execute, shared_from(this)));
    }
    END_TRY_PARAM(boost::asio::post(boost::bind(&http_session::send_json, m_session, m_writer.stringify())))
}

bool create_tx_base_handler::check_json(http_json_rpc_request_ptr request, json_rpc_id id)
{
    BGN_TRY
    {
        json_rpc_reader reader;
        CHK_PRM(reader.parse(request->get_result().c_str()), "Invalid response json")

        json_rpc_id _id = reader.get_id();
        CHK_PRM(_id != 0 && _id == id, "Returned id doesn't match")

        auto err = reader.get_error();
        auto res = reader.get_result();

        CHK_PRM(err || res, "No occur result or error")

        if (err)
        {
            this->m_writer.set_error(*err);
            return false;
        }

        return true;
    }
    END_TRY_RET(false)
}
