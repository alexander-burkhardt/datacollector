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

# Recursively collect all .cpp files from src/
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Create executable
add_executable(datacollector ${SOURCES})

# Target include directories (allows #include from any src/ subdirectory)
target_include_directories(datacollector PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Link libraries (using async version for MQTT)
target_link_libraries(datacollector PRIVATE eclipse-paho-mqtt-c::paho-mqtt3a)

# Create test executable
add_executable(test_runner tests/TestExample.cpp)
target_include_directories(test_runner PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
add_test(NAME AllTests COMMAND test_runner)
