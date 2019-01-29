#include "get_tx_handler.h"
#include "exception/except.h"

// get_tx_handler
bool get_tx_handler::prepare_params()
{
    BGN_TRY
    {
        CHK_PRM(m_id, "id field not found")
        CHK_PRM(false, "method has not implemented")
        return false;
    }
    END_TRY_RET(false)
}
