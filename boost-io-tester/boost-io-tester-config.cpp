#include "boost-io-tester-config.h"
#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

bool CBoostIoTesterConfig::read_config()
{
    bool ret = false;
    try {
        boost::property_tree::ptree tree;
        boost::property_tree::json_parser::read_json("config.json", tree);

        // Access data in the property tree
        weather_server_appid = tree.get<std::string>("weather_server_appid");
        ret = true;
    }
    catch (std::exception const& e) {
        std::cerr << e.what() << std::endl;
    }
    return ret;
}
