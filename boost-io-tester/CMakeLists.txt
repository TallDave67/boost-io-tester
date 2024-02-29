# CMakeList.txt : CMake project for boost-io-tester, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (boost-io-tester "boost-io-tester.cpp" "boost-io-tester.h" "boost-io-tester-config.cpp" "boost-io-tester-config.h" "boost-io-tester-asio.cpp" "boost-io-tester-asio.h" "test_ids.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET boost-io-tester PROPERTY CXX_STANDARD 20)
endif()

# Find OpenSSL
find_package(OpenSSL REQUIRED)
set(OPENSSL_ROOT $ENV{OPENSSL_ROOT})
include_directories(${OPENSSL_ROOT})
target_link_libraries(boost-io-tester PRIVATE OpenSSL::SSL)

# TODO: Add tests and install targets if needed.
