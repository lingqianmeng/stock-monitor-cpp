#include <iostream>
#include <string>
#include <curl/curl.h>
#include "core/sendSMS.hpp"
#include "core/configSecrets.hpp" // for config variable

bool sendSMS(const std::string& message, const std::string& to_number) {
    CURL* curl;
    CURLcode res;
    bool success = false;

    // --- 配置区域 (请替换为你自己的 Twilio 信息) ---
    const std::string account_sid = config.twilio_account_sid; // 你的账户 SID
    const std::string auth_token  = config.twilio_auth_token; // 你的认证令牌
    const std::string from_number = config.twilio_phone_number; // Twilio 给你的号码
    // ---------------------------------------------

    // Twilio API URL
    std::string url = "https://api.twilio.com/2010-04-01/Accounts/" + account_sid + "/Messages.json";
    
    // 构建 POST 数据: To, From, Body
    std::string post_fields = "To=" + to_number + "&From=" + from_number + "&Body=" + message;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        
        // 设置为 POST 请求
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_fields.c_str());

        // 设置认证 (Basic Auth)
        curl_easy_setopt(curl, CURLOPT_USERNAME, account_sid.c_str());
        curl_easy_setopt(curl, CURLOPT_PASSWORD, auth_token.c_str());

        // 执行请求
        res = curl_easy_perform(curl);

        if(res != CURLE_OK) {
            std::cerr << "[SMS Error] send SMS failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            long http_code = 0;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
            if (http_code == 201) { // 201 Created 代表发送成功
                std::cout << "[SMS] SMS sent successfully!" << std::endl;
                success = true;
            } else {
                std::cerr << "[SMS Error] API returned error code: " << http_code << std::endl;
            }
        }
        curl_easy_cleanup(curl);
    }
    return success;
}