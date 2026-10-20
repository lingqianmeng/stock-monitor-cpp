[![Build and Release](https://github.com/lingqianmeng/stock-monitor-cpp/actions/workflows/Build_Release.yml/badge.svg)](https://github.com/lingqianmeng/stock-monitor-cpp/actions/workflows/Build_Release.yml)
# Stock Monitor C++ 📈
![C++](https://img.shields.io/badge/C++-20-blue.svg) ![License](https://img.shields.io/badge/license-MIT-green.svg) ![Build](https://img.shields.io/badge/build-CMake-orange.svg) ![Dependencies](https://img.shields.io/badge/vcpkg-enabled-purple.svg)

A high-performance, multi-threaded C++ application designed to monitor real-time stock prices. The system fetches live market data via **Finnhub API** and triggers instant **SMS notifications** through **Twilio** when a target price threshold is reached.

The project is built and secured by a **DevSecOps pipeline in GitHub Actions**: Windows build, static code analysis, secret scanning, vulnerability scanning, a hardened multi-stage Docker image and a security-gated release flow.

## ✨ Key Features

* **Real-time Monitoring**: High-frequency price polling using `libcurl` for efficient REST API communication.
* **Multi-threaded Engine**: Each stock is monitored in its own independent thread, ensuring no latency interference between different assets.
* **SMS Alerts**: Seamless integration with the Twilio API to provide mobile notifications even when the app is running in the background.
* **Professional Dependency Management**: Uses `vcpkg` in Manifest Mode for reproducible and easy-to-setup development environments.
* **Secure Credential Management**: Sensitive API keys are isolated in a locally stored `secrets.json` file protected by .gitignore, ensuring there is no threat of private credentials being revealed to version control.
* **🆕 CI/CD with GitHub Actions:** Automated Windows build, Linux Docker build and tagged releases.
* **🆕 DevSecOps Security Checks:** CodeQL, Gitleaks and Trivy run on every pull request and push to `main`.
* **🆕 Hardened Container Image:** Multi-stage Docker build running as a non-root user (UID 10001).
## 🛠️ Technical Stack

* **Language**: C++20
* **Network**: [libcurl](https://curl.se/libcurl/) (HTTP/HTTPS requests)
* **Data Parsing**: [nlohmann/json](https://github.com/nlohmann/json) (JSON serialization/deserialization)
* **Build System**: CMake (>= 3.15)
* **Package Manager**: vcpkg (Manifest Mode)
* **Notification Service**: Twilio REST API
* **Stock Price**: Finnhub and FMP APIs
* **🆕 CI/CD:** GitHub Actions
* **🆕 Container:** Docker (multi-stage, Ubuntu 24.04)
* **🆕 Security Tooling:** CodeQL, Gitleaks, Trivy

## 🚀 Quick Start

### 1. Prerequisites

Ensure the following tools are installed on your system:

- [CMake](https://cmake.org/download/) (>= 3.15)
- C++ compiler (MSVC / Visual Studio 2022 Build Tools, GCC or Clang)
- [Git](https://git-scm.com/)
- [vcpkg](https://github.com/microsoft/vcpkg) with the environment variable `VCPKG_ROOT` set (one-time setup):

```powershell
cd C:\dev
git clone https://github.com/microsoft/vcpkg.git
.\vcpkg\bootstrap-vcpkg.bat
[Environment]::SetEnvironmentVariable("VCPKG_ROOT", "C:\dev\vcpkg", "User")
```

> Reopen your terminal afterwards so that `VCPKG_ROOT` is available.
> The location of vcpkg is up to you – CMake finds it via `VCPKG_ROOT`.

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

### 5. Build with Docker (optional)

The Linux build runs in a multi-stage Docker image. Dependencies come from `vcpkg.json`, the same as in the Windows build.

```bash
docker build -t stock-monitor-cpp .
```

- **Build stage:** compiler, CMake, Ninja and vcpkg build the application.
- **Runtime stage:** only the binary and the required runtime libraries are copied into a minimal Ubuntu 24.04 image.
- The container runs as the **non-root user UID 10001**.
* The secrets.json file should be included together with exe within the same folder or at least same project folder. As long as the secrets.json file is less than two layers higher than the directory of exe, then it can be loaded into program.

## 🔄 CI/CD Pipeline

All jobs are defined in [`.github/workflows/Build_Release.yml`](.github/workflows/Build_Release.yml).

**Triggers:** push to `main`, tags `v*`, pull requests to `main` and manual runs (**Run workflow**).

| Job | Runner | Purpose |
|---|---|---|
| `build-windows` | `windows-latest` | Builds `StockMonitor.exe` with CMake + vcpkg (static triplet) and runs **CodeQL** (C++, `security-extended`) |
| `secrets-scan` | `ubuntu-latest` | **Gitleaks** scans the full Git history for leaked API keys |
| `repo-scan` | `ubuntu-latest` | **Trivy** filesystem scan of the repository (category `trivy-fs`) |
| `docker-build` | `ubuntu-latest` | Builds the multi-stage image, runs smoke tests and the **Trivy image scan** (category `trivy-image`) |
| `create-release` | `ubuntu-latest` | Creates a GitHub Release on `v*` tags, **only after all security checks have passed** |

### Smoke tests (`docker-build`)

- The container starts.
- `StockMonitor` exists and is executable.
- `ldd` finds no missing libraries.
- The container runs as **UID 10001** (non-root); the job fails otherwise.
- The image size is printed in every run.

### Security results

All findings are uploaded as SARIF to **Security → Code scanning**.

| Tool | Scope | Category |
|---|---|---|
| CodeQL | Own C++ source code | CodeQL |
| Trivy | Repository (dependencies, secrets, misconfigurations) | `trivy-fs` |
| Trivy | Docker image, CRITICAL/HIGH CVEs with an available fix | `trivy-image` |
| Gitleaks | Git history | Job log |

### Optional full image scan

The regular image scan reports only CRITICAL/HIGH CVEs with an available fix. A full overview (all severities, including unfixed CVEs) can be started on demand. The output appears only in the Actions log and is **not** uploaded to Code scanning.

| Situation | How to start it |
|---|---|
| Pull request | Add the label `full-scan` |
| Merge to `main` | Add `[full-scan]` to the merge commit message |
| Manual | **Actions → Build and Release → Run workflow** → tick *Run full Trivy scan* |

### Supply-chain hardening

- Third-party actions such as Trivy are **pinned to a commit SHA**.
- `codeql-action` uses **v4**.
- Workflow permissions default to `contents: read`; write permissions are granted per job only where needed.

## 🛡️ Branch Protection

`main` is protected by the ruleset `protect-main`:

- Changes only via pull request from a feature branch (`ci/...`, `build/...`, `fix/...`).
- Required status checks: `build-windows`, `CodeQL`, `secrets-scan`, `repo-scan`, `Docker Build`.

## 📊 Results of the Multi-stage Build

Measured with the full Trivy scan (all severities, including unfixed CVEs):

| | Before (single-stage) | After (multi-stage) | Reduction |
|---|---|---|---|
| **Image size** | 1.49 GB | 96.7 MB | **−93.5 %** |
| **CVEs total** | 2824 | 25 | **−99.1 %** |
| CRITICAL | 0 | 0 | – |
| HIGH | 78 | 0 | −100 % |
| MEDIUM | 2553 | 21 | −99.2 % |
| LOW | 193 | 4 | −97.9 % |

The remaining 25 CVEs come from the Ubuntu 24.04 base packages; none have a fix available yet.

## 📦 Releases

Push a tag to build and publish a release:

```bash
git tag v1.0.0
git push origin v1.0.0
```

The Windows executable `StockMonitor.exe` is attached to the GitHub Release once all build and security checks are green.
