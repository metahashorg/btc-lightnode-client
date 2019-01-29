#ifndef SEND_TX_HANDLER_H
#define SEND_TX_HANDLER_H

#include "create_tx_base_handler.h"

class send_tx_handler : public create_tx_base_handler
{
public:
    send_tx_handler(http_session_ptr& session);
};

#endif // SEND_TX_HANDLER_H
