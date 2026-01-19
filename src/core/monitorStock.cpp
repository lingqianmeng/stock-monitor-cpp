#include "core/monitorStock.hpp"
#include "core/getStockPrice.hpp"
#include "core/sendSMS.hpp"
#include "main.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>

// Only one thread can hold the lock at a time to print to console
std::mutex consoleMutex;

void monitorStock(const int& choice, const std::string& symbol, const double targetPrice, const AppConfig& config) {
    double currentPrice = 0.0;
    while (!stopFlag) {
        // --- THE PROTECTED SECTION ---
        {
        // The mutex is locked here
        std::lock_guard<std::mutex> lock(consoleMutex); 
        if (choice == DAX) {
            std::cout << "[DAX] Checking price for " << symbol << "..." << std::endl;
            std::cout << "Target Price: Euro " << targetPrice << std::endl;
            currentPrice = getStockPriceDAX(symbol, config.FMP_api_key);
            std::cout << "Current price: Euro " << currentPrice << std::endl;
        }  else if (choice == NASDAQ) {
            std::cout << "[NASDAQ] Checking price for " << symbol << "..." << std::endl;
            std::cout << "Target Price: US-Dollar " << targetPrice << std::endl;
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
        std::cout << "----------------------------------------" <<  std::endl;
        // The mutex is automatically unlocked here
        }
        // --- END OF THE PROTECTED SECTION ---

        // sleep outside the locked section to allow other threads to proceed
        std::this_thread::sleep_for(std::chrono::seconds(waitingTime));
    }
}