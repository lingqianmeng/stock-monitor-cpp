#pragma once
#include <string>
#include "configSecrets.hpp"
#include "main.hpp"

void monitorStockThreadWrapper(const Stock& stock, const Stocks& stocks, const AppConfig& config);
void monitorStock(const int choice, const std::string& symbol, const double targetPrice, const int waitingTime, const AppConfig& config);
