#include <fstream>
#include "core/loadStocks.hpp"
#include <nlohmann/json.hpp>
#include <iostream>
#include <stdexcept>
#include <filesystem> // C++17 feature
#include "main.hpp"
using json = nlohmann::json;
namespace fs = std::filesystem;


bool loadStocks(Stocks& stocksToLoad, int stockMarket) {
    std::string filename = "stocks.json";
    fs::path filePath = fs::current_path() / filename;

    if (!fs::exists(filePath)) {
        filePath = fs::current_path().parent_path() / filename;
    }

    if (!fs::exists(filePath)) {
        filePath = fs::current_path().parent_path().parent_path() / filename;
    }

    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return false;
    }

    try {
        json data = json::parse(file);
        
        if (stockMarket == NASDAQ) {
            // 1. Load General Settings
            stocksToLoad.update_interval_seconds = data["settings_Nasdaq"]["update_interval_seconds"];
            // 2. Load Watchlist into Vector
            for (const auto& item : data["watchlist_Nasdaq"]) {
                stocksToLoad.stocks.push_back({
                    item["market"].get<int>(),
                    item["symbol"].get<std::string>(),
                    item["target"].get<double>()
                });
            }
        } else if (stockMarket == DAX) {
            // 1. Load General Settings
            stocksToLoad.update_interval_seconds = data["settings_DAX"]["update_interval_seconds"];
            // 2. Load Watchlist into Vector
            for (const auto& item : data["watchlist_DAX"]) {
                stocksToLoad.stocks.push_back({
                    item["market"].get<int>(),
                    item["symbol"].get<std::string>(),
                    item["target"].get<double>()
                });
            }
        } else {
            throw std::runtime_error("Unknown stock market identifier");
        }
       
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Config Error: " << e.what() << std::endl;
        std::cerr << "Failed to load stocks from stocks.json file." << std::endl;
        return false;
    }

    return true;
}