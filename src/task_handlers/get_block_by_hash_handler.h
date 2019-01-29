#ifndef GET_BLOCK_BY_HASH_HANDLER_H_
#define GET_BLOCK_BY_HASH_HANDLER_H_

#include "network_handler.h"

class get_block_by_hash_handler : public base_network_handler
{
public:
    get_block_by_hash_handler(http_session_ptr session)
        : base_network_handler(settings::server::address, session)
    {}
    get_block_by_hash_handler()
        : base_network_handler(settings::server::address)
    {}

protected:
    virtual bool prepare_params() override;
};

#endif // GET_BLOCK_BY_HASH_HANDLER_H_