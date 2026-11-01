# The toolbox: one compiler for each of the three targets, running on your
# laptop's own CPU (x86_64 or Apple Silicon), so building is never emulated.
FROM debian:bookworm-slim

RUN set -eux; \
    case "$(dpkg --print-architecture)" in \
      amd64) other="gcc-aarch64-linux-gnu libc6-dev-arm64-cross" ;; \
      arm64) other="gcc-x86-64-linux-gnu libc6-dev-amd64-cross" ;; \
      *) echo "This laptop CPU is not supported: $(dpkg --print-architecture)" >&2; exit 1 ;; \
    esac; \
    apt-get update; \
    apt-get install -y --no-install-recommends \
        gcc libc6-dev make file python3 \
        gcc-arm-linux-gnueabihf libc6-dev-armhf-cross $other; \
    rm -rf /var/lib/apt/lists/*

WORKDIR /work
CMD ["bash"]
