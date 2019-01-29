#include "get_dump_block_by_hash_handler.h"
#include "exception/except.h"

// get_dump_block_by_hash
bool get_dump_block_by_hash::prepare_params()
{
    BGN_TRY
    {
        CHK_PRM(m_id, "id field not found")
        CHK_PRM(false, "method has not implemented")
        return false;
    }
    END_TRY_RET(false)
}
