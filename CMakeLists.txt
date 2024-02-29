# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.28)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("boost-io-tester")

# Find boost
set(BOOST_ROOT $ENV{BOOST_ROOT})
set(BOOST_VERSION  $ENV{BOOST_VERSION})
set(boost_system_DIR "${BOOST_ROOT}/stage/lib/cmake/boost_system-${BOOST_VERSION}")
find_package(boost_system ${BOOST_VERSION} REQUIRED CONFIG)
set(boost_fiber_DIR "${BOOST_ROOT}/stage/lib/cmake/boost_fiber-${BOOST_VERSION}")
find_package(boost_fiber ${BOOST_VERSION} REQUIRED CONFIG)
include_directories(${BOOST_ROOT})
set(boost_lib_DIR "${BOOST_ROOT}/stage/lib")
link_directories(${boost_lib_DIR})

# Include sub-projects.
add_subdirectory ("boost-io-tester")
