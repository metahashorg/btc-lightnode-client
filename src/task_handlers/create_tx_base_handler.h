#ifndef CREATE_TX_BASE_HANDLER_H
#define CREATE_TX_BASE_HANDLER_H

//#include <string>
#include "network_handler.h"

#include "http_session_ptr.h"
#include "http_json_rpc_request_ptr.h"
#include "../btc_wallet/BtcWallet.h"

class create_tx_base_handler : public base_network_handler
{
public:
    create_tx_base_handler(http_session_ptr& session);
    virtual ~create_tx_base_handler() override;

protected:
    virtual bool prepare_params() override;

    bool get_transaction_params();
    void on_get_transaction_params(http_json_rpc_request_ptr req, json_rpc_id id);

    void build_request();
    bool check_json(http_json_rpc_request_ptr request, json_rpc_id id);

protected:
    bool                        m_auto_fee = {false};
    bool                        m_all_value = {false};
    std::unique_ptr<BtcWallet>  m_btc_wallet;
    std::string                 m_address;
    std::string                 m_password;
    std::string                 m_to;
    std::string                 m_nonce;
    std::string                 m_fee;
    std::string                 m_value;
    std::string                 m_estimate_fee;
    std::vector<BtcInput>       m_utxos;
};

#endif // CREATE_TX_BASE_HANDLER_H
