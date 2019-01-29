#include "generate_handler.h"
#include "settings/settings.h"
#include "../btc_wallet/BtcWallet.h"

// generate_handler
bool generate_handler::prepare_params()
{
    BGN_TRY
    {
        CHK_PRM(m_id, "id field not found")

        auto params = m_reader.get_params();
        CHK_PRM(params, "params field not found")

        CHK_PRM(m_reader.get_value(*params, "password", m_password), "password field not found")
        CHK_PRM(!m_password.empty(), "empty password")

        for (auto& c: m_password)
        {
            CHK_PRM(c >= 0x20 && c <= 0x7e, "password contains not allowed symbols")
        }

        return true;
    }
    END_TRY_RET(false)
}

void generate_handler::execute()
{
    BGN_TRY
    {
        const std::string address = BtcWallet::genPrivateKey(settings::system::wallet_stotage, m_password).first;
        m_writer.add_result("address", address);
    }
    END_TRY
}
