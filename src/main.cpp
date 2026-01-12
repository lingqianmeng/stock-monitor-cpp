#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <csignal>
#include "getStockPrice.hpp"
#include "sendSMS.hpp"
#include "configSecrets.hpp"

bool volatile stopFlag = false;
#define waitingTime 30  // check price every 30 seconds
#define DAX         1
#define NASDAQ      2

// define global config variable, declared in configSecrets.hpp
AppConfig config;

// signal handler to catch Ctrl+C/interrupt
void signalHandler(int signum) {
    std::cout << "\n received intrrupt signal (" << signum << "), exiting..." << std::endl;
    stopFlag = true;
}

int main() {
    std::string symbol;
    double targetPrice;
    int choice;
    double currentPrice = 0.0;

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

    // handle Ctrl+C
    signal(SIGINT, signalHandler);

    while (!stopFlag) {
        if (choice == DAX){
            std::cout << "[DAX] Checking price for " << symbol << "..." << std::endl;
            currentPrice = getStockPriceDAX(symbol, config.FMP_api_key);
            std::cout << "Current price: Euro " << currentPrice << std::endl;
        }  else if (choice == NASDAQ) {
            std::cout << "[NASDAQ] Checking price for " << symbol << "..." << std::endl;
            currentPrice = getStockPriceNASDAQ(symbol, config.finnhub_api_key);
            std::cout << "Current price: US-Dollar " << currentPrice << std::endl;
        }      

        if (currentPrice >= targetPrice) {
            std::cout << " Target price triggered! Sending notification..." << std::endl;
            // send SMS notification
            std::string message = "Alert: " + symbol + " has reached the target price of " + std::to_string(targetPrice) + ". Current price: " + std::to_string(currentPrice);
            sendSMS(message, config.my_phone_number); 
            break; 
        }

        // check price every waitingTime seconds to avoid rate limit of free API
        std::cout << "Waiting for next check after " << waitingTime << " seconds..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(waitingTime));
    }

    return 0;
}