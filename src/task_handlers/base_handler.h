#ifndef BASE_HANDLER_H
#define BASE_HANDLER_H

#include <memory>

#include "http_session_ptr.h"
#include "json_rpc.h"
#include "log/log.h"
#include "settings/settings.h"
#include "exception/except.h"
#include "utils.h"

using mh_count_t = uint64_t;

struct handler_result
{
    operator bool() const                 {return !pending;}
    operator const std::string&() const   {return message;}
    operator const char*() const          {return message.c_str();}
    std::string message;
    bool pending = { false };
};

class base_handler: public std::enable_shared_from_this<base_handler>
{
public:
    base_handler();
    base_handler(http_session_ptr& session);

    virtual ~base_handler();

    virtual void execute() = 0;
    bool prepare(const std::string& params);
    handler_result result();

protected:
    virtual bool prepare_params() = 0;

    template <typename T>
    std::shared_ptr<T> shared_from(T*) {
        return std::static_pointer_cast<T>(shared_from_this());
    }

protected:
    handler_result          m_result;
    json_rpc_reader         m_reader;
    json_rpc_writer         m_writer;
    http_session_ptr        m_session;
    utils::time_duration    m_duration;
    json_rpc_id             m_id = { 0 };
};

#endif // BASE_HANDLER_H
