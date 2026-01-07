#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include "getStockPrice.hpp"

using json = nlohmann::json;

// callback function to write received data into a string
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}


//get stock price from API 
double getStockPrice(const std::string& symbol, const std::string& apiKey) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if(curl) {
        // Finnhub API endpoint for stock quote
        std::string url = "https://finnhub.io/api/v1/quote?symbol=" + symbol + "&token=" + apiKey;
        
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if(res == CURLE_OK) {
            auto data = json::parse(readBuffer);
            // "c" stands for Current Price in Finnhub API
            return data["c"].get<double>();
        }
    }
    return -1.0;
}