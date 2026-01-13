#include "core/monitorStock.hpp"
#include "core/getStockPrice.hpp"
#include "core/sendSMS.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include "main.hpp"

void monitorStock(int& choice, const std::string& symbol, double targetPrice, AppConfig& config) {
    double currentPrice = 0.0;
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

}