#include "get_last_txs_handler.h"

// get_last_txs_handler
bool get_last_txs_handler::prepare_params()
{
    BGN_TRY
    {
        CHK_PRM(m_id, "id field not found")
        CHK_PRM(false, "method has not implemented")
        return false;
    }
    END_TRY_RET(false)
}
