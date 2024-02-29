#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>

class CBoostIoTesterConfig;

class CBoostIoTesterAsio
{
public:
    void get_weather(CBoostIoTesterConfig* pConfig, const std::string& lat, const std::string& lon);
    void get_space_image(boost::asio::io_context& io_context, const std::string& host, const std::string& target, const std::string& filename, boost::asio::yield_context yield);

};