cmake_minimum_required(VERSION 3.10)

# Save the vcpkg path here
set(CMAKE_TOOLCHAIN_FILE "C:/Users/binyi/projects/dependency/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

project(StockMonitor)

# Since your headers are in src/core, we tell CMake to look there.
# This fixes "fatal error: InventoryManager.h: No such file or directory"
include_directories(src/core)

# find and link CURL and nlohmann_json
find_package(CURL REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

# Define core logic files (Everything EXCEPT main.cpp)
file(GLOB CORE_SOURCES "src/core/*.cpp")

add_executable(StockMonitor 
    src/main.cpp 
    ${CORE_SOURCES}       # Include all src/core files here
)

# Link the libraries
target_link_libraries(StockMonitor PRIVATE CURL::libcurl nlohmann_json::nlohmann_json)



