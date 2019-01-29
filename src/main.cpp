#include <iostream>

#include "http_server.h"
#include "settings/settings.h"
#include "log/log.h"

namespace po = boost::program_options;
namespace bs = boost::system;

static std::unique_ptr<http_server> server;

void print_request_help();
void input_handler();

int main(int argc, char* argv[])
{
    try
    {
        logg::init();
        settings::read();

        po::options_description desc("Allowed options");
        desc.add_options()
            ("help",                                    "produce help message")
            ("request",                                 "list of allowed requests with a detailed description")
            ("threads",     po::value<int>(),           "number of threads")
            ("port",        po::value<unsigned short>(),"service port, default is 9999")
            ("server",      po::value<std::string>(),   "server address")
            ("storage",     po::value<std::string>(),   "storage of wallets")
            ("any",                                     "accept any connections")
            ("token",       po::value<std::string>(),  	"sevice token")
            ("coin-key",    po::value<int>(),           "coin key identificator");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help"))
        {
            std::cout << desc << std::endl;
            return EXIT_SUCCESS;
        }

        if (vm.count("request"))
        {
            print_request_help();
            return EXIT_SUCCESS;
        }

        settings::read(vm);

        std::thread input_thr(input_handler);

        server = std::make_unique<http_server>(settings::service::port, settings::service::threads);
        server->run();

        input_thr.join();

        return EXIT_SUCCESS;
    }
    catch (const std::exception& e)
    {
        logg::push_err(e.what());
        return EXIT_FAILURE;
    }
}

void print_request_help()
{
    po::options_description info("Requests:");
    info.add_options()
        ("Generate wallet",     "{\"id\":decimal/str, \"version\":\"2.0\",\"method\": \"generate\", \"params\":{\"password\": str}}")
        ("Balance of wallet",   "{\"id\":decimal/str, \"version\":\"2.0\",\"method\": \"fetch-balance\", \"params\":{\"address\": str}}")
        ("History of wallet",   "{\"id\":decimal/str, \"version\":\"2.0\",\"method\": \"fetch-history\", \"params\":{\"address\": str}}")
        ("Create transaction",  "{\"id\":decimal/str, \"version\":\"2.0\",\"method\": \"create-tx\", \"params\":{\"address\": str, \"password\": str, \"to\": str, \"value\": \"decimal/all\", \"fee\": \"decimal/auto\"}}")
        ("Send transaction",    "{\"id\":decimal/str, \"version\":\"2.0\",\"method\": \"send-tx\", \"params\":{\"address\": str, \"password\": str, \"to\": str, \"value\": \"decimal/all\", \"fee\": \"decimal/auto\"}}");

    std::cout << info << std::endl;
}

void input_handler()
{
    while (true)
    {
        std::string in;
        std::cin >> in;
        if (in.compare("stop") == 0)
        {
            server->stop();
            break;
        }
    }
}
