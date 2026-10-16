# ---------- Stage 1: build (with vcpkg, as before) ----------
FROM ubuntu:24.04 AS build
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential cmake ninja-build git curl zip unzip tar \
    pkg-config ca-certificates \
    && rm -rf /var/lib/apt/lists/*

RUN git clone https://github.com/microsoft/vcpkg.git /opt/vcpkg && \
    /opt/vcpkg/bootstrap-vcpkg.sh -disableMetrics

WORKDIR /app
COPY . .
RUN cmake -B build -S . -G Ninja \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_TOOLCHAIN_FILE=/opt/vcpkg/scripts/buildsystems/vcpkg.cmake \
 && cmake --build build

# ---------- Stage 2: runtime (small, non-root) ----------
FROM ubuntu:24.04

RUN apt-get update && apt-get install -y --no-install-recommends ca-certificates \
    && rm -rf /var/lib/apt/lists/* \
    && useradd --system --uid 10001 --no-create-home appuser

COPY --from=build /app/build/StockMonitor /usr/local/bin/StockMonitor

USER appuser
CMD ["StockMonitor"]