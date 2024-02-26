// boost-io-tester.cpp : Defines the entry point for the application.
//

#include "boost-io-tester.h"
#include "boost-io-tester-config.h"
#include "boost-io-tester-asio.h"
#include "test_ids.h"

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
        // Add more cases for additional test programs

    default:
        std::cerr << "Invalid test_id. Please provide a valid test_id." << std::endl;
        return 1;
    }

    return 0;
}
