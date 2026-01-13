#include <fstream>
#include "core/configSecrets.hpp"
#include <nlohmann/json.hpp>
#include <iostream>
#include <stdexcept>
#include <filesystem> // C++17 feature

using json = nlohmann::json;
namespace fs = std::filesystem;

bool loadSecrets(AppConfig& config) {
    std::string filename = "secrets.json";

    // try to locate the secrets.json file in multiple possible directories
    fs::path filePath = fs::current_path() / filename;
    // 1. If not found in current folder, check the parent folder
    if (!fs::exists(filePath)) {
        filePath = fs::current_path().parent_path() / filename;
    }
    // 2. If still not found, check the parent of the parent (e.g., from build/Debug to root)
    if (!fs::exists(filePath)) {
        filePath = fs::current_path().parent_path().parent_path() / filename;
    }

    std::ifstream file(filePath);
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