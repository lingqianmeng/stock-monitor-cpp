cmake_minimum_required(VERSION 3.10)

# Save the vcpkg path here
set(CMAKE_TOOLCHAIN_FILE "C:/Users/binyi/projects/dependency/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

project(StockMonitor)

# 寻找包
find_package(CURL REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(StockMonitor 
main.cpp
getStockPrice.cpp
sendSMS.cpp
configSecrets.cpp
)

# 自动链接库
target_link_libraries(StockMonitor PRIVATE CURL::libcurl nlohmann_json::nlohmann_json)



