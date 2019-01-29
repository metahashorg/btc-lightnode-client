#ifndef GET_LAST_TXS_HANDLER_H_
#define GET_LAST_TXS_HANDLER_H_

#include "network_handler.h"

class get_last_txs_handler : public base_network_handler
{
public:
    get_last_txs_handler(http_session_ptr session)
        : base_network_handler(settings::server::address, session)
    {}
    get_last_txs_handler()
        : base_network_handler(settings::server::address)
    {}

protected:
    virtual bool prepare_params() override;
};

#endif // GET_LAST_TXS_HANDLER_H_
