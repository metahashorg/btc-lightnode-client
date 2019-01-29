#ifndef GET_BLOCKS_HANDLER_H_
#define GET_BLOCKS_HANDLER_H_

#include "network_handler.h"

class get_blocks_handler : public base_network_handler
{
public:
    get_blocks_handler(http_session_ptr session)
        : base_network_handler(settings::server::address, session)
    {}
    get_blocks_handler()
        : base_network_handler(settings::server::address)
    {}

protected:
    virtual bool prepare_params() override;
};

#endif // GET_BLOCKS_HANDLER_H_
