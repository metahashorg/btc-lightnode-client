#ifndef _FETCH_HISTORY_HANDLER_H_
#define _FETCH_HISTORY_HANDLER_H_

#include "network_handler.h"

class fetch_history_handler : public base_network_handler
{
public:
    fetch_history_handler(http_session_ptr& session)
        : base_network_handler(settings::server::address, session)
    {}
    fetch_history_handler()
        : base_network_handler(settings::server::address)
    {}

protected:
    virtual bool prepare_params() override;
};

#endif // _FETCH_HISTORY_HANDLER_H_
