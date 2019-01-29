#include "get_block_by_hash_handler.h"

// get_block_by_hash_handler
bool get_block_by_hash_handler::prepare_params()
{
    BGN_TRY
    {
        CHK_PRM(m_id, "id field not found")
        CHK_PRM(false, "method has not implemented")
        return false;
    }
    END_TRY_RET(false)
}
