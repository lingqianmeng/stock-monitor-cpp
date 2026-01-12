#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include "getStockPrice.hpp"
#include <iostream>

using json = nlohmann::json;

// callback function to write received data into a string
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}


//get stock price from API 
double getStockPriceDAX(const std::string& symbol, const std::string& apiKey) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if(curl) {
        // FMP API endpoint for stock quote
        std::string url = "https://financialmodelingprep.com/stable/profile?symbol=" + symbol + "&apikey=" + apiKey;
        
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        
        // Follow redirects if necessary
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        
        // Set the callback function to write the response data into our string
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            try {
                // Parse the JSON (FMP returns an array of objects for the quote endpoint)
                auto jsonData = json::parse(readBuffer);

                if (!jsonData.empty()) {
                    auto stock = jsonData[0]; // Access first object in array
                    /*
                    std::cout << "--- Stock Quote: " << stock["symbol"] << " ---" << std::endl;
                    std::cout << "Name:  " << stock["name"] << std::endl;
                    std::cout << "Price: " << stock["price"] << " EUR" << std::endl;
                    std::cout << "Change: " << stock["change"] << " (" << stock["changesPercentage"] << "%)" << std::endl;
                    */
                    return stock["price"].get<double>();
                } else {
                    std::cout << "No data found for symbol: " << symbol << std::endl;
                }
            } catch (json::parse_error& e) {
                std::cerr << "JSON Parse Error: " << e.what() << std::endl;
            }
        }

        // Cleanup
        curl_easy_cleanup(curl);
    }
    return -1.0;
}

//get stock price from API 
double getStockPriceNASDAQ(const std::string& symbol, const std::string& apiKey) {
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