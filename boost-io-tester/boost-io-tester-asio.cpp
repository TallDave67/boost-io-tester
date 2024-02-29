#include "boost-io-tester-asio.h"
#include "boost-io-tester-config.h"

//using namespace std::chrono_literals;

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

void CBoostIoTesterAsio::get_space_image(boost::asio::io_context& io_context, const std::string& host, const std::string& target, const std::string& filename, boost::asio::yield_context yield) {
    try {
        // SSL context and socket creation
        boost::asio::ssl::context ctx(boost::asio::ssl::context::tlsv12_client);
        boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket(io_context, ctx);

        // DNS resolution and connection
        boost::asio::ip::tcp::resolver resolver(io_context);
        auto const results = resolver.resolve(host, "https");
        boost::asio::connect(socket.next_layer(), results.begin(), results.end());
        socket.handshake(boost::asio::ssl::stream_base::client);

        // HTTP request
        boost::beast::http::request<boost::beast::http::empty_body> req(boost::beast::http::verb::get, target, 11);
        req.set(boost::beast::http::field::host, host);
        req.set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);

        // Send the request
        boost::beast::http::async_write(socket, req, yield);

        // Read the response
        boost::beast::flat_buffer buffer;
        boost::beast::http::response<boost::beast::http::string_body> res;
        boost::beast::http::async_read(socket, buffer, res, yield);

        // Check if response is successful and content type is image/png
        if (res.result() == boost::beast::http::status::ok && res[boost::beast::http::field::content_type] == "image/png") {
            // Write the response to a file
            std::ofstream outfile(filename, std::ios::binary);
            outfile << res.body();
            outfile.close();
            std::cout << "Saved PNG file: " << filename << std::endl;
        }
        else {
            std::cerr << "Error: Received response is not a PNG file or request failed." << std::endl;
        }

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}