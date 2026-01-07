#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <csignal>
#include "getStockPrice.hpp"

bool volatile stopFlag = false;
#define waitingTime 30  // check price every 30 seconds



// signal handler to catch Ctrl+C/interrupt
void signalHandler(int signum) 
{
    std::cout << "\n received intrrupt signal (" << signum << "), exiting..." << std::endl;
    stopFlag = true;
}

int main() 
{
    std::string apiKey; // apply personal API key from Finnhub
    std::string symbol;
    double targetPrice;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << " Finnhub API Key: ";
    std::cin >> apiKey;
    std::cout << " Code of Stock (e.g. AAPL): ";
    std::cin >> symbol;
    std::cout << " The alarm price: ";
    std::cin >> targetPrice;
    std::cout << "Monitoring " << symbol << "... Target price: " << targetPrice << std::endl;
    std::cout << "Press Ctrl+C to stop." << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    // handle Ctrl+C
    signal(SIGINT, signalHandler);

    while (!stopFlag) 
    {
        double currentPrice = getStockPrice(symbol, apiKey);
        
        if (currentPrice > 0) 
        {
            std::cout << "Current price: $" << currentPrice << std::endl;

            if (currentPrice >= targetPrice) 
            {
                std::cout << " Target price triggered! Sending notification..." << std::endl;
                // Todo: add code to send email or SMS notification
                break; 
            }
        }

        // check price every waitingTime seconds to avoid rate limit of free API
        std::cout << "Waiting for next check after " << waitingTime << " seconds..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(waitingTime));
    }

    return 0;
}