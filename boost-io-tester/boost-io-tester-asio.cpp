#include "boost-io-tester-asio.h"
#include "boost-io-tester-config.h"
#include <boost/asio.hpp>
#include <iostream>

void CBoostIoTesterAsio::get_weather(CBoostIoTesterConfig* pConfig, const std::string& lat, const std::string& lon)
{
    if (pConfig)
    {
        try {
            boost::asio::io_service io_service;
            boost::asio::ip::tcp::resolver resolver(io_service);
            boost::asio::ip::tcp::resolver::query query("api.openweathermap.org", "http");

            boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
            boost::asio::ip::tcp::socket socket(io_service);
            boost::asio::connect(socket, endpoint_iterator);

            // Send HTTP GET request
            std::string request = "GET /data/2.5/weather?lat=" + lat + "&lon=" + lon + "&appid=" + pConfig->weather_server_appid + "&units=metric HTTP/1.1\r\n";
            request += "Host: api.openweathermap.org\r\n";
            request += "Connection: close\r\n\r\n";
            boost::asio::write(socket, boost::asio::buffer(request));

            // Read response
            std::cout << "Response: ";
            for (;;)
            {
                std::array<char, 128> buf;
                boost::system::error_code error;

                size_t len = socket.read_some(boost::asio::buffer(buf), error);
                if (error == boost::asio::error::eof)
                    break; // Connection closed cleanly by peer.
                else if (error)
                    throw boost::system::system_error(error); // Some other error.

                std::cout.write(buf.data(), len);
            }
            std::cout << std::endl;
        }
        catch (std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
}
