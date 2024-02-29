// boost-io-tester.cpp : Defines the entry point for the application.
//

#include "boost-io-tester.h"
#include "boost-io-tester-config.h"
#include "boost-io-tester-asio.h"
#include "test_ids.h"

#include <boost/asio.hpp>
#include <boost/fiber/all.hpp>

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <test_id> ..." << std::endl;
        return 1;
    }

    // Convert the test_id argument to an integer
    int test_id = std::stoi(argv[1]);
    if (test_id == TEST_GET_WEATHER && argc != 4) {
        std::cerr << "Usage to get the weather: " << argv[0] << " <test_id=100> <lat> <lon>" << std::endl;
        return 1;
    }

    // Launch the corresponding test program based on the test_id
    switch (test_id) {
    case TEST_GET_WEATHER:
        {
            CBoostIoTesterConfig config;
            config.read_config();
            CBoostIoTesterAsio asio;
            asio.get_weather(&config, argv[2], argv[3]);
        }
        break;
    case TEST_GET_SPACE_IMAGE:
    {
        CBoostIoTesterConfig config;
        config.read_config();
        //
        std::string host = "api.nasa.gov";
        std::string path1 = "/EPIC/archive/natural/2024/02/28/png/epic_1b_20240228110604.png?api_key=" + config.space_server_appid;
        std::string path2 = "/EPIC/archive/natural/2019/05/30/png/epic_1b_20190530011359.png?api_key=" + config.space_server_appid;
        std::string filename1 = "output/epic_1b_20240228110604.png";
        std::string filename2 = "output/epic_1b_20190530011359.png";

        boost::asio::io_context io_context;

        // Launching asynchronous HTTPS GET requests
        CBoostIoTesterAsio asio;
        boost::asio::spawn(io_context, [&](boost::asio::yield_context yield) {
            asio.get_space_image(io_context, host, path1, filename1, yield);
            });

        boost::asio::spawn(io_context, [&](boost::asio::yield_context yield) {
            asio.get_space_image(io_context, host, path2, filename2, yield);
            });

        // Running the io_context
        io_context.run();
    }
    break;
    // Add more cases for additional test programs

    default:
        std::cerr << "Invalid test_id. Please provide a valid test_id." << std::endl;
        return 1;
    }

    return 0;
}
