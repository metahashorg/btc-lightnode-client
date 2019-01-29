#include "task_handlers.h"

#include "generate_handler.h"
#include "create_tx_handler.h"
#include "send_tx_handler.h"
#include "fetch_history_handler.h"
#include "fetch_balance_handler.h"
#include "get_tx_handler.h"
#include "get_blocks_handler.h"
#include "get_dump_block_by_hash_handler.h"
#include "get_count_blocks_handler.h"
#include "get_last_txs_handler.h"
#include "get_block_by_hash_handler.h"
#include "get_block_by_number_handler.h"
#include "get_blocks_handler.h"
#include "get_dump_block_by_number_handler.h"

template <class T>
static handler_result perform(http_session_ptr& session, const std::string& params) {
    try {
        std::shared_ptr<T> obj = std::make_shared<T>(session);
        if (obj->prepare(params)) {
            obj->execute();
        }
        return obj->result();
    } catch (std::exception& ex) {
        LOG_ERR("handler perform exception %s: %s", __PRETTY_FUNCTION__, ex.what())
        return handler_result();
    } catch (...) {
        LOG_ERR("handler perform unhandled exception %s", __PRETTY_FUNCTION__)
        return handler_result();
    }
}

const std::map<std::string, handler_func> map_handlers = {
    { "generate",               perform<generate_handler> },
    { "create-tx",              perform<create_tx_handler> },
    { "send-tx",                perform<send_tx_handler> },
    { "get-count-blocks",       perform<get_count_blocks_handler> },    // no impl
    { "get-last-txs",           perform<get_last_txs_handler> },        // no impl
    { "get-block-by-hash",      perform<get_block_by_hash_handler> },   // no impl
    { "get-block-by-number",    perform<get_block_by_number_handler> }, // no impl
    { "get-blocks",             perform<get_blocks_handler> },          // no impl
    { "get-dump-block-by-number", perform<get_dump_block_by_number> },  // no impl
    { "get-dump-block-by-hash",	perform<get_dump_block_by_hash> },      // no impl
    { "get-tx",                 perform<get_tx_handler> },              // no impl
    { "fetch-balance",          perform<fetch_balance_handler> },
    { "fetch-history",          perform<fetch_history_handler> }
};
