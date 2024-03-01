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
    else if (test_id == TEST_GET_SPACE_IMAGES && argc < 3) {
        std::cerr << "Usage to get the weather: " << argv[0] << " <test_id=99> <image1> ..." << std::endl;
        return 1;
    }

    // Read the config
    CBoostIoTesterConfig config;
    config.read_config();

    // Create our asio communicator
    CBoostIoTesterAsio asio;

    // Launch the corresponding test program based on the test_id
    switch (test_id) {
    case TEST_GET_WEATHER:
        {
            asio.get_weather(&config, argv[2], argv[3]);
        }
        break;
    case TEST_GET_SPACE_IMAGES:
    {
        std::vector<std::string> filenames;
        for (int arg = 2; arg < argc; arg++)
        {
            filenames.push_back({argv[arg]});
        }
        asio.get_space_images(&config, filenames);
    }
    break;
    // Add more cases for additional test programs

    default:
        std::cerr << "Invalid test_id. Please provide a valid test_id." << std::endl;
        return 1;
    }

    return 0;
}
