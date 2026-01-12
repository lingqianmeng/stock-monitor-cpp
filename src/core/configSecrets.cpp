#include <fstream>
#include "configSecrets.hpp"
#include <nlohmann/json.hpp>
#include <iostream>
#include <stdexcept>

using json = nlohmann::json;

bool loadSecrets(AppConfig& config) {
    
    std::ifstream file("secrets.json");
    if (!file.is_open()) {
        throw std::runtime_error("Could not open secrets file: secrets.json");
        return false;
    }   
    
    try {
        json data = json::parse(file);
        config.twilio_account_sid = data["twilio_account_sid"];
        config.twilio_auth_token = data["twilio_auth_token"];
        config.twilio_phone_number = data["twilio_phone_number"];
        config.my_phone_number = data["my_phone_number"];
        config.finnhub_api_key = data["finnhub_api_key"];
        config.FMP_api_key = data["FMP_api_key"];
    } catch (const std::exception& e) {
        std::cerr << "load secrets.json failed: " << e.what() << std::endl;
        return false;
    }

    return true;
}