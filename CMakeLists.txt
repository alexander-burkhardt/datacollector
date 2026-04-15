cmake_minimum_required(VERSION 3.10)
project(DataCollector)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable testing
enable_testing()

# Find Paho MQTT (C library, usable in C++)
find_package(eclipse-paho-mqtt-c CONFIG REQUIRED)

# Find Google Test
find_package(GTest CONFIG REQUIRED)
include(GoogleTest)

# Recursively collect all .cpp files from src/
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE IMPL_SOURCES CONFIGURE_DEPENDS "src/*.cpp")
list(FILTER IMPL_SOURCES EXCLUDE REGEX "src/main\\.cpp$")

# Create executable
add_executable(datacollector ${SOURCES})

# Target include directories (allows #include from any src/ subdirectory)
target_include_directories(datacollector PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Link libraries (using async version for MQTT)
target_link_libraries(datacollector PRIVATE eclipse-paho-mqtt-c::paho-mqtt3a)

# Create test executable
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS "tests/*.cpp")
add_executable(test_runner ${TEST_SOURCES} ${IMPL_SOURCES})
target_include_directories(test_runner PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/tests)
target_link_libraries(test_runner PRIVATE GTest::gtest_main)
gtest_discover_tests(test_runner)
