#ifndef GET_DUMP_BLOCK_BY_HASH_HANDLER_H_
#define GET_DUMP_BLOCK_BY_HASH_HANDLER_H_

#include "network_handler.h"

class get_dump_block_by_hash : public base_network_handler
{
public:
    get_dump_block_by_hash(http_session_ptr session)
        : base_network_handler(settings::server::address, session)
    {}
    get_dump_block_by_hash()
        : base_network_handler(settings::server::address)
    {}

protected:
    virtual bool prepare_params() override;
};

#endif // GET_DUMP_BLOCK_BY_HASH_HANDLER_H_
