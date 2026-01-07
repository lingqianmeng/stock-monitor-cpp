#pragma once
#include <string>

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp);
double getStockPrice(const std::string& symbol, const std::string& apiKey);