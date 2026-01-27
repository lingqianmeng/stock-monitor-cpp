#include <iostream>
#include <string>
#include <thread>
#include <csignal>
#include <vector>
#include "core/getStockPrice.hpp"
#include "core/sendSMS.hpp"
#include "core/configSecrets.hpp"
#include "main.hpp"
#include "core/monitorStock.hpp"
#include "core/loadStocks.hpp"

// define global stop flag, declared in main.hpp
volatile bool stopFlag = false;

// define global config variable, declared in configSecrets.hpp
AppConfig config;

Stocks daxStocks;
Stocks nasdaqStocks;

// signal handler to catch Ctrl+C/interrupt
void signalHandler(int signum) {
    std::cout << "\n received intrrupt signal (" << signum << "), exiting..." << std::endl;
    stopFlag = true;
}

int main() {
    std::string symbol;
    double targetPrice;
    int choice; 
    try {
        if (!loadStocks(daxStocks, DAX)) {
            return 1;
        } 
    } catch (const std::exception& e) {
        std::cerr << "FATAL ERROR: " << e.what() << std::endl;
        std::cerr << "Failed to load DAX stocks" << std::endl;
        return 1;
    }

    try {
        if (!loadStocks(nasdaqStocks, NASDAQ)) {
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << "FATAL ERROR: " << e.what() << std::endl;
        std::cerr << "Failed to load NASDAQ stocks" << std::endl;
        return 1;
    }
    
    // load secrets from file
    // handle exceptions if file not found or parsing error
    try {
        if (!loadSecrets(config)) {
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << "FATAL ERROR: " << e.what() << std::endl;
        std::cerr << "Make sure secrets.json is in the same folder you are running from!" << std::endl;
        return 1;
    }

    // handle Ctrl+C
    signal(SIGINT, signalHandler);

    std::cout << "----------------------------------------" << std::endl;
    std::cout << " Welcome to Stock Monitor!" << std::endl;

    std::vector<std::thread> threads;

    for (const auto& stock : daxStocks.stocks) {
       // threads.push_back(std::thread(monitorStock, std::cref(stock), std::cref(daxStocks), std::cref(config)));
        threads.push_back(std::thread(monitorStockThreadWrapper, std::cref(stock), std::cref(daxStocks), std::cref(config)));
    }

    for (const auto& stock : nasdaqStocks.stocks) {
        threads.push_back(std::thread(monitorStockThreadWrapper, std::cref(stock), std::cref(nasdaqStocks), std::cref(config)));
    }

    // Keep program alive
    for (auto& t : threads) {
        if (t.joinable()) t.join();
    }

    /*Ver 2.0.0 
    std::cout << "----------------------------------------" << std::endl;
    std::cout << " Welcome to Stock Monitor!" << std::endl;
    std::cout << " Choose your stock market: " << std::endl;
    std::cout << " 1. DAX (Germany)" << std::endl;
    std::cout << " 2. NASDAQ (USA)" << std::endl;
    std::cout << " Your choice (1-2): ";
    std::cin >> choice;
    std::cout << "----------------------------------------" << std::endl;
    if (choice == DAX) {
        std::cout << "You have chosen DAX!" << std::endl;
    } else if (choice == NASDAQ) {
        std::cout << "You have chosen NASDAQ!" << std::endl;
    } else {
        std::cout << " Invalid choice. Exiting." << std::endl;
        return 1;
    }
    std::cout << " Code of Stock (e.g. AAPL): ";
    std::cin >> symbol;
    std::cout << " The alarm price: ";
    std::cin >> targetPrice;
    std::cout << "Monitoring " << symbol << "... Target price: " << targetPrice << std::endl;
    std::cout << "Press Ctrl+C to stop..." << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    monitorStock(choice, symbol, targetPrice, config);
    */



    return 0;
}