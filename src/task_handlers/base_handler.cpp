#include "base_handler.h"

base_handler::base_handler(http_session_ptr& session):
    m_session(session),
    m_duration(false)
{
}

base_handler::base_handler():
    m_duration(false)
{
}

base_handler::~base_handler() {
    m_duration.stop();
}

handler_result base_handler::result() {
    m_result.message = m_writer.stringify();
    return m_result;
}

bool base_handler::prepare(const std::string& params)
{
    BGN_TRY
    {
        m_duration.start();

        CHK_PRM(m_reader.parse(params.c_str()), "Parse error")

        m_id = m_reader.get_id();
        m_writer.set_id(m_id);

        CHK_PRM(settings::service::coin_key != -1, "Coin-key is not present")

        CHK_PRM(!settings::service::token.empty(), "Token is not present")

        const bool success = prepare_params();
        const bool pending = m_result.pending;
        if (!success && !pending)
        {
            // prepare_params must set an error

            if (!m_writer.is_error())
            {
                m_writer.reset();
                m_writer.set_error(-32602, "Invalid params");
            }
        }

        LOG_DBG("Prepared json (%u,%u): %s", success, pending, m_writer.stringify().c_str())

        return success;
    }
    END_TRY_RET(false)
}
