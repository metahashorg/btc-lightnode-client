#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string>
#include <sstream>
#include "../log/log.h"
#include "../btc_wallet/TypedException.h"

class invalid_param
{
public:
    invalid_param(std::string message): m_msg(message)
    {
        std::stringstream ss;
        ss << __PRETTY_FUNCTION__ << " in file " << __FILE__ << " at line " << __LINE__;
        m_where = ss.str();
    }

    ~invalid_param() {}

    std::string what() { return m_msg; }
    std::string where() { return m_where; }

protected:
    std::string m_msg;
    std::string m_where;
};

#define CHK_PRM(condition, message) \
    if (!(condition)) {\
        std::ostringstream stream;\
        stream << message;\
        stream.flush();\
        throw invalid_param(stream.str()); }

#define BGN_TRY try

#define END_TRY_RET_PARAM(ret, param) \
    catch (TypedException& e)\
    {\
        LOG_ERR("Exception \"%s\" in func %s", e.description.c_str(), __PRETTY_FUNCTION__)\
        this->m_writer.reset();\
        this->m_writer.set_error(-32669, e.description);\
        param;\
        return ret;\
    }\
    catch (invalid_param& ex)\
    {\
        LOG_ERR("Exception \"%s\" in func %s", ex.what().data(), __PRETTY_FUNCTION__)\
        this->m_writer.reset();\
        this->m_writer.set_error(-32668, ex.what().data());\
        param;\
        return ret;\
    }\
    catch (std::exception& ex)\
    {\
        LOG_ERR("Exception \"%s\" in func %s", ex.what(), __PRETTY_FUNCTION__)\
        this->m_writer.reset();\
        this->m_writer.set_error(-32667, ex.what());\
        param;\
        return ret;\
    }\
    catch(...)\
    {\
        LOG_ERR("Unhandled exception in func %s", __PRETTY_FUNCTION__)\
        this->m_writer.reset();\
        this->m_writer.set_error(-32666, "Unhandled exception");\
        param;\
        return ret;\
    }

#define END_TRY_RET(ret)\
    END_TRY_RET_PARAM(ret, )

#define END_TRY_PARAM(param)\
    END_TRY_RET_PARAM(,param)

#define END_TRY\
    END_TRY_RET()

#endif // EXCEPTION_H
