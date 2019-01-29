#ifndef GET_TX_HANDLER_H_
#define GET_TX_HANDLER_H_

#include "network_handler.h"

class get_tx_handler : public base_network_handler
{
public:
    get_tx_handler(http_session_ptr session)
        : base_network_handler(settings::server::address, session)
    {}

    get_tx_handler()
        : base_network_handler(settings::server::address)
    {}

protected:
    virtual bool prepare_params() override;
};

#endif // GET_TX_HANDLER_H_
