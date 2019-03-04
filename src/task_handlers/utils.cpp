#include <stdarg.h>
#include "utils.h"
#include "../log/log.h"
#include <iostream>

namespace utils
{
    void hex2mpz(std::string src, mpz_class& dst)
    {
        if (src.empty())
            return;
        if (src.compare(0, 2, "0x") == 0)
            src.erase(0, 2);
        dst.set_str(src, 16);
    }

    void mpz2hex(mpz_class& src, std::string& dst)
    {
        dst = src.get_str(16);
        if (dst.empty())
        {
            dst = "0x0";
        }
        else
        {
            if (dst.compare(0, 2, "0x") != 0)
                dst.insert(0, "0x");
        }
    }

    void parse_address(const std::string& address, std::string& host, std::string& port, std::string& path, bool& use_ssl)
    {
        std::string tmp = address;

        auto pos = tmp.find("http://");
        if (pos != std::string::npos)
        {
            port.clear();
            port = "80";
            tmp = tmp.substr(pos + 7);
        }

        pos = tmp.find("https://");
        if (pos != std::string::npos)
        {
            port.clear();
            port = "443";
            tmp = tmp.substr(pos + 8);
            use_ssl = true;
        }

        pos = tmp.find(":");
        if (pos != std::string::npos)
        {
            port.clear();
            port = tmp.substr(pos + 1);
            tmp = tmp.substr(0, pos);
        }

        pos = tmp.find("/");
        if (pos != std::string::npos)
        {
            path.clear();
            path = tmp.substr(pos);
            tmp = tmp.substr(0, pos);
        }

        host = tmp;
    }

    // Timer
    Timer::~Timer()
    {
        stop();
    }

    void Timer::start(const Interval& interval, const Handler& handler, bool immediately /*= true*/)
    {
        m_handler = handler;
        m_interval = interval;
        if (immediately)
            run_once();
    }

    void Timer::stop()
    {
        std::lock_guard<std::mutex> guard(m_locker);
        m_handler = nullptr;
    }

    void Timer::run_once()
    {
        m_promise = std::promise<void>();
        m_thr = std::thread([&]()
        {
            auto fut = m_promise.get_future();
            if (fut.wait_for(m_interval) == std::future_status::timeout)
            {
                std::lock_guard<std::mutex> guard(m_locker);
                if (m_handler) {
                    m_handler();
                    m_handler = nullptr;
                }
            }
        });
        m_thr.detach();
    }

    // time_duration
    time_duration::time_duration(bool _start):
        m_run(false)
    {
        if (_start)
            start();
    }

    time_duration::time_duration(bool _start, std::string message):
        m_run(false),
        m_msg(message)
    {
        if (_start)
            start();
    }

    time_duration::~time_duration()
    {
        stop();
    }

    void time_duration::start()
    {
        if (!m_run)
        {
            m_run = true;
            m_start = boost::posix_time::microsec_clock::local_time();
        }
    }

    void time_duration::stop()
    {
        if (m_run)
        {
            m_run = false;
            boost::posix_time::ptime end = boost::posix_time::microsec_clock::local_time();
            boost::posix_time::time_duration dur = end - m_start;
            long msec = dur.total_milliseconds();
            LOG_DBG("%s: %u millisec", m_msg.c_str(), msec)
        }
    }
}
