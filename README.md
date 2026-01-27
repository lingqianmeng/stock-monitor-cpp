[![Build and Release](https://github.com/lingqianmeng/stock-monitor-cpp/actions/workflows/Build_Release.yml/badge.svg)](https://github.com/lingqianmeng/stock-monitor-cpp/actions/workflows/Build_Release.yml)
# Stock Monitor C++ 📈
![C++](https://img.shields.io/badge/C++-17-blue.svg) ![License](https://img.shields.io/badge/license-MIT-green.svg) ![Build](https://img.shields.io/badge/build-CMake-orange.svg) ![Dependencies](https://img.shields.io/badge/vcpkg-enabled-purple.svg)

A high-performance, multi-threaded C++ application designed to monitor real-time stock prices. The system fetches live market data via **Finnhub API** and triggers instant **SMS notifications** through **Twilio** when a target price threshold is reached.



## ✨ Key Features

* **Real-time Monitoring**: High-frequency price polling using `libcurl` for efficient REST API communication.
* **Multi-threaded Engine**: Each stock is monitored in its own independent thread, ensuring no latency interference between different assets.
* **SMS Alerts**: Seamless integration with the Twilio API to provide mobile notifications even when the app is running in the background.
* **Professional Dependency Management**: Uses `vcpkg` in Manifest Mode for reproducible and easy-to-setup development environments.
* **Secure Credential Management**: Sensitive API keys are isolated in a locally stored `secrets.json` file protected by .gitignore, ensuring there is no threat of private credentials being revealed to version control.

## 🛠️ Technical Stack

* **Language**: C++17
* **Network**: [libcurl](https://curl.se/libcurl/) (HTTP/HTTPS requests)
* **Data Parsing**: [nlohmann/json](https://github.com/nlohmann/json) (JSON serialization/deserialization)
* **Build System**: CMake (>= 3.15)
* **Package Manager**: vcpkg (Manifest Mode)
* **Notification Service**: Twilio REST API
* **Stock Price**: Finnhub and FMP APIs

## 🚀 Quick Start

### 1. Prerequisites

Ensure the following tools are installed on your system:
* CMake
* C++ Compiler (GCC, Clang, or MSVC)
* [vcpkg](https://github.com/microsoft/vcpkg)

### 2. Configuration (API Keys)

You will need to obtain API keys, Token and phone number from the following providers for free:
* **Stock Data Nasdaq**: [Finnhub](https://finnhub.io/) -> API Key
* **Stock Data DAX**: [FMP](https://site.financialmodelingprep.com/) -> API Key
* **SMS Service**: [Twilio](https://www.twilio.com/) -> Account SID, Auth Token, and a Twilio phone number -> Register for Whatsapp messages with your own phone number.

### 3. Build Instructions

```bash
# Clone the repository
git clone https://github.com/lingqianmeng/stock-monitor-cpp.git
cd stock-monitor-cpp

# Configure project
cmake -S . -B build

# Build project
cmake --build build
```

### 4. Configuration (secrets.json)

The last step is to configure the secrets.json file 
  ```bash
  cp secrets.example.json secrets.json
  ```
* Fill in the real values: Open secrets.json and replace the placeholders with your actual data from Twilio, Finnhub and FMP and your own number for Whatsapp.
* Check your .gitignore: Ensure secrets.json is listed so you don't accidentally push it.
* The secrets.json file should be included together with exe within the same folder or at least same project folder. As long as the secrets.json file is less than two layers higher than the directory of exe, then it can be loaded into program.

