# syntax=docker/dockerfile:1

FROM gcc:13-bookworm AS builder

RUN apt-get update && apt-get install -y --no-install-recommends \
    cmake \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src
COPY . .

RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release \
    && cmake --build build -j"$(nproc)"

FROM gcc:13-bookworm

WORKDIR /app
COPY --from=builder /src/build/NetSim /src/build/NetSimTests ./
COPY --from=builder /src/examples ./examples

CMD ["./NetSimTests"]
