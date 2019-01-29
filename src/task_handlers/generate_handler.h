#ifndef GENERATE_HANDLER_H
#define GENERATE_HANDLER_H

#include "base_handler.h"

class generate_handler : public base_handler
{
public:
    generate_handler(http_session_ptr session): base_handler(session)
    {
        std::stringstream ss;
        ss << __FUNCTION__;
        m_duration.set_message(ss.str());
    }

    virtual bool prepare_params() override;
    virtual void execute() override;

protected:
    std::string m_password;
};

#endif // GENERATE_HANDLER_H
