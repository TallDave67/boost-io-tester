#include "boost-io-tester-asio.h"
#include "boost-io-tester-config.h"

void CBoostIoTesterAsio::get_weather(CBoostIoTesterConfig* pConfig, const std::string& lat, const std::string& lon)
{
    if (pConfig)
    {
        // create request
        std::string request = "GET /data/2.5/weather?lat=" + lat + "&lon=" + lon + "&appid=" + pConfig->weather_server_appid + "&units=metric HTTP/1.1\r\n";

        // send request and get response
        get_text_response_for_sync_request(request);
    }
}

void CBoostIoTesterAsio::get_space_images(CBoostIoTesterConfig* pConfig, const std::vector<std::string>& filenames) {
    if (pConfig)
    {
        boost::asio::io_context io_context;

        // Make request per filename
        std::vector<std::string> paths;
        std::vector<std::string> filepaths;
        std::size_t index = 0;
        for (; index < filenames.size(); index++)
        {
            const std::string& filename = filenames[index];
            std::string year, month, day;
            get_date_components(filename, year, month, day);
            std::string path = "/EPIC/archive/natural/" + year + "/" + month + "/" + day + "/png/" + filename + "?api_key=" + pConfig->space_server_appid;
            paths.push_back(path);
            std::string filepath = "output/" + filename;
            filepaths.push_back(filepath);

            // spawn thread to make request
            // note that we are capturing all variables by reference except the index as each thread needs the correct unique index
            boost::asio::spawn(io_context, [this, &io_context, &paths, &filepaths, index](boost::asio::yield_context yield) {
                get_image_respone_for_secure_async_request(io_context, paths[index], filepaths[index], yield);
            });
        }

        // Running the io_context
        io_context.run();
    }
}

void CBoostIoTesterAsio::get_text_response_for_sync_request(std::string& request)
{
    try {
        boost::asio::io_service io_service;
        boost::asio::ip::tcp::resolver resolver(io_service);
        std::string host = "api.openweathermap.org";
        boost::asio::ip::tcp::resolver::query query(host, "http");

        boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
        boost::asio::ip::tcp::socket socket(io_service);
        boost::asio::connect(socket, endpoint_iterator);

        // Add the host to the request
        request += "Host: ";
        request += host;
        request += "\r\n";
        request += "Connection: close\r\n\r\n";

        // Send HTTP GET request
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

void CBoostIoTesterAsio::get_image_respone_for_secure_async_request(boost::asio::io_context& io_context, const std::string& path, const std::string& filepath, boost::asio::yield_context yield)
{
    try {
        // SSL context and socket creation
        boost::asio::ssl::context ctx(boost::asio::ssl::context::tlsv12_client);
        boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket(io_context, ctx);

        // DNS resolution and connection
        boost::asio::ip::tcp::resolver resolver(io_context);
        std::string host = "api.nasa.gov";
        auto const results = resolver.resolve(host, "https");
        boost::asio::connect(socket.next_layer(), results.begin(), results.end());
        socket.handshake(boost::asio::ssl::stream_base::client);

        // HTTP request
        std::cout << "path to get = " << path << std::endl;
        boost::beast::http::request<boost::beast::http::empty_body> req(boost::beast::http::verb::get, path, 11);
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
            std::ofstream outfile(filepath, std::ios::binary);
            outfile << res.body();
            outfile.close();
            std::cout << "Saved PNG file: " << filepath << std::endl;
        }
        else {
            std::cerr << "Error: Received response is not a PNG file or request failed." << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void CBoostIoTesterAsio::get_date_components(const std::string& filename, std::string& year, std::string& month, std::string& day)
{
    // filename of form "epic_1b_YYYYMMDD..."
    year = filename.substr(8, 4);
    month = filename.substr(12, 2);
    day = filename.substr(14, 2);
}
