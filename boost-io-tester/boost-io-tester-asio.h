#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>

class CBoostIoTesterConfig;

class CBoostIoTesterAsio
{
public:
    void get_weather(CBoostIoTesterConfig* pConfig, const std::string& lat, const std::string& lon);
    void get_space_images(CBoostIoTesterConfig* pConfig, const std::vector<std::string>& filenames);

private:
    // communicators
    void get_text_response_for_sync_request(std::string& request);
    void get_image_respone_for_secure_async_request(boost::asio::io_context& io_context, const std::string& path, const std::string& filepath, boost::asio::yield_context yield);

    // helpers
    void get_date_components(const std::string& filename, std::string& year, std::string& month, std::string& day);
};