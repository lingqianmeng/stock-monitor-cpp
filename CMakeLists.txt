cmake_minimum_required(VERSION 3.10)

# Save the vcpkg path here
set(CMAKE_TOOLCHAIN_FILE "C:/Users/binyi/projects/dependency/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

project(StockMonitor)

# Set C++17 standard to support filesystem library
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Since your headers are in src/core, we tell CMake to look there.
# This fixes "fatal error: InventoryManager.h: No such file or directory" 
# 18.01.2026: "old-school" way to include directories. Instead, we will use target_include_directories below.
# include_directories(src/core)

# find and link CURL and nlohmann_json
find_package(CURL REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

# Define core logic files (Everything EXCEPT main.cpp)
file(GLOB CORE_SOURCES "src/core/*.cpp")

add_executable(StockMonitor 
    src/main.cpp 
    ${CORE_SOURCES}       # Include all src/core files here
)

# Include the src directory for header files
# src/core is included automatically since we used GLOB for CORE_SOURCES
target_include_directories(StockMonitor PRIVATE src)

# Link the libraries
target_link_libraries(StockMonitor PRIVATE CURL::libcurl nlohmann_json::nlohmann_json)



