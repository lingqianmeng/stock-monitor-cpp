#pragma once
#include <string>
#include "configSecrets.hpp"

void monitorStock(const int& choice, const std::string& symbol, const double targetPrice, const AppConfig& config);