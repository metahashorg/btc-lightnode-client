#ifndef CREATE_TX_HANDLER_H
#define CREATE_TX_HANDLER_H

#include "create_tx_base_handler.h"

class create_tx_handler : public create_tx_base_handler
{
public:
    create_tx_handler(http_session_ptr& session)
        : create_tx_base_handler(session) {}

    virtual void execute() override;
};

#endif // CREATE_TX_HANDLER_H
