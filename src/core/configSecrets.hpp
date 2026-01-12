#pragma once
#include <string>

struct AppConfig {
    std::string twilio_account_sid;
    std::string twilio_auth_token;
    std::string twilio_phone_number;
    std::string my_phone_number;
    std::string finnhub_api_key;
    std::string FMP_api_key;
};

bool loadSecrets(AppConfig& config);

// declare global config variable, defined in main.cpp
extern AppConfig config;
