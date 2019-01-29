#ifndef SETTINGS_H
#define SETTINGS_H

#define BOOST_ERROR_CODE_HEADER_ONLY
#include <boost/program_options.hpp>

#include <string>

namespace settings
{
    struct service
    {
        static bool any_conns;
        static unsigned short port;
        static int threads;
        static std::string token;
        static int coin_key;
        static std::vector<std::string> access;
    };

    struct server
    {
        static std::string address;
    };

    struct system
    {
        static std::string wallet_stotage;
    };

    void read();
    void read(boost::program_options::variables_map& vm);
}

#endif // SETTINGS_H
