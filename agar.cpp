#ifdef _WIN32
    #include <sdkddkver.h>
#endif

#include <iostream>

#include <boost/program_options.hpp>

#include <server/Server.h>

namespace po = boost::program_options;

int main(int argc, char const* argv[])
{
    po::options_description description("Program options");
    description.add_options()
            ("address,a", po::value<std::string>()->default_value("127.0.0.1"), "server IP address")
            ("help,h", "show this message")
            ("port,p", po::value<unsigned short>()->default_value(32168), "server transport layer port")
            ;
    po::variables_map variablesMap;
    po::store(po::parse_command_line(argc, argv, description), variablesMap);
    po::notify(variablesMap);

    if (variablesMap.count("help"))
    {
        std::cout << description << std::endl;
        return 0;
    }

    auto address = variablesMap["address"].as<std::string>();
    auto  port = variablesMap["port"].as<unsigned short>();

    Server server(address, port);

    // TODO: add signal handler
    server.run();

    return 0;
}
