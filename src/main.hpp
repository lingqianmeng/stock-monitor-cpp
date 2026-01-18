
#pragma once
#include <string>
#include <vector>

extern volatile bool stopFlag;

#define waitingTime 30  // check price every 30 seconds
#define DAX         1
#define NASDAQ      2

struct Stock {
    int stockMarket;
    std::string symbol;
    double targetPrice;
};

struct Stocks {
    int update_interval_seconds;
    std::string currency;
    std::vector<Stock> stocks;
};
