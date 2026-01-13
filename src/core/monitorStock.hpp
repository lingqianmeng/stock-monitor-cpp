#pragma once
#include <string>
#include "configSecrets.hpp"

void monitorStock(int& choice, const std::string& symbol, double targetPrice, AppConfig& config);