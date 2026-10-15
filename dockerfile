# 1) Base Image: ein leeres Ubuntu 24.04
FROM ubuntu:24.04

# 2) Verhindert, dass apt-get beim Build auf Eingaben wartet
ENV DEBIAN_FRONTEND=noninteractive

# 3) Build-Werkzeuge installieren (Compiler, CMake, Ninja, Tools für vcpkg)
RUN apt-get update && apt-get install -y \
    build-essential cmake ninja-build git curl zip unzip tar \
    pkg-config ca-certificates \
    && rm -rf /var/lib/apt/lists/*

# 4) vcpkg in den Container holen und einrichten
RUN git clone https://github.com/microsoft/vcpkg.git /opt/vcpkg && \
    /opt/vcpkg/bootstrap-vcpkg.sh -disableMetrics

# 5) Arbeitsordner im Container und Quellcode hineinkopieren
WORKDIR /app
COPY . .

# 6) Konfigurieren und bauen. vcpkg liest vcpkg.json und installiert curl und nlohmann-json automatisch
RUN cmake -B build -S . -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_TOOLCHAIN_FILE=/opt/vcpkg/scripts/buildsystems/vcpkg.cmake
RUN cmake --build build

# 7) Was beim Start des Containers passiert (vorerst nur ein Test)
CMD ["echo", "StockMonitorCpp container built successfully"]