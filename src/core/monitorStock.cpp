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

void monitorStock(const int choice, const std::string& symbol, const double targetLowPrice, const double targetHighPrice, const int waitingTime, const AppConfig& config) {
    double currentPrice = 0.0;
    bool alertSentLow = false;
    bool alertSentHigh = false;
    const double priceBuffer = 0.01; // small buffer to prevent multiple alerts

    while (!stopFlag) {
        // --- THE PROTECTED SECTION ---
        {
        // The mutex is locked here
        std::lock_guard<std::mutex> lock(consoleMutex); 
        try {
            if (choice == DAX) {
            std::cout << "[DAX] Checking price for " << symbol << "..." << std::endl;
            std::cout << "Target Price Low: Euro " << targetLowPrice << std::endl;
            std::cout << "Target Price High: Euro " << targetHighPrice << std::endl;
            currentPrice = getStockPriceDAX(symbol, config.FMP_api_key);
            std::cout << "Current price: Euro " << currentPrice << std::endl;
            }  else if (choice == NASDAQ) {
            std::cout << "[NASDAQ] Checking price for " << symbol << "..." << std::endl;
            std::cout << "Target Price Low: US-Dollar " << targetLowPrice << std::endl;
            std::cout << "Target Price High: US-Dollar " << targetHighPrice << std::endl;
            currentPrice = getStockPriceNASDAQ(symbol, config.finnhub_api_key);
            std::cout << "Current price: US-Dollar " << currentPrice << std::endl;
            } 
        } catch (const std::exception& e) {
            std::cerr << "[ERROR] getStockPrice failed for " << symbol << ": " << e.what() << std::endl;
            continue; // skip this iteration on error
        }
             
        try {
            if (currentPrice >= targetHighPrice) {
                if (!alertSentHigh) {
                    std::cout << " Target price triggered! Sending notification..." << std::endl;
                    std::string message = "Alert: " + symbol + " has reached the target HIGH price of " + std::to_string(targetHighPrice) + ". Current price: " + std::to_string(currentPrice);
                    sendSMS(message, config.my_phone_number); // send SMS/Whatapp notification
                    alertSentHigh = true; // set alert flag to avoid multiple notifications
                }
            } else if (currentPrice < targetHighPrice*(1-priceBuffer)) {
                alertSentHigh = false; // reset alert flag when price goes below target high
            }
        
            if (currentPrice <= targetLowPrice) {
                if (!alertSentLow) {
                    std::cout << " Target price triggered! Sending notification..." << std::endl;
                    std::string message = "Alert: " + symbol + " has dropped to the target LOW price of " + std::to_string(targetLowPrice) + ". Current price: " + std::to_string(currentPrice);
                    sendSMS(message, config.my_phone_number); // send SMS/Whatapp notification
                    alertSentLow = true; // set alert flag to avoid multiple notifications
                }
            } else if (currentPrice > targetLowPrice*(1+priceBuffer)) {
                alertSentLow = false; // reset alert flag when price goes above target low
            }    
        } catch (const std::exception& e) {
            std::cerr << "[ERROR] sendSMS for " << symbol << ": " << e.what() << std::endl;
            continue; // skip this iteration on error
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

void monitorStockThreadWrapper(const Stock& stock, const Stocks& stocks, const AppConfig& config) {
    monitorStock(stock.stockMarket, stock.symbol, stock.targetLowPrice, stock.targetHighPrice, stocks.update_interval_seconds, config);
}