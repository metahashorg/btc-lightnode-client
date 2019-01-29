#include "get_dump_block_by_number_handler.h"

// get_dump_block_by_number
bool get_dump_block_by_number::prepare_params()
{
    BGN_TRY
    {
        CHK_PRM(m_id, "id field not found")
        CHK_PRM(false, "method has not implemented")
        return false;
    }
    END_TRY_RET(false)
}
