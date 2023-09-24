// #define FMT_HEADER_ONLY // may need this line

#include <iostream>

#include <cxxopts.hpp>
#include <fmt/format.h>

#include "config.hpp"

int main(int argc, char **argv)
{


    const auto welcome_message =
        fmt::format("Welcome to {} v{}\n", project_name, project_version);

    cxxopts::Options options(project_name.data(), welcome_message);

    options.add_options()         // add options
        ("h,help", "Print usage") // help
        ("i,integer", "Int param", cxxopts::value<uint32_t>());

    auto result = options.parse(argc, argv);

    if (argc == 1 || result.count("help"))
    {
        std::cout << options.help() << '\n';
        return 0;
    }

    return 0;
}
