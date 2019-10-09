#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

#include <openssl/sha.h>
#include <string>
#include <vector>

#define DO_BENCHMARK(title, msg_size)                  \
    {                                                  \
        std::vector<unsigned char> msg(msg_size, 123); \
        unsigned char md[32];                          \
        BENCHMARK(title)                               \
        {                                              \
            SHA256(msg.data(), msg.size(), md);        \
        };                                             \
    }

TEST_CASE("sha256", "benchmarking")
{
    DO_BENCHMARK("msg.size=1KB", 1 << 10);
    DO_BENCHMARK("msg.size=4KB", 1 << 12);
    DO_BENCHMARK("msg.size=16KB", 1 << 14);
    DO_BENCHMARK("msg.size=64KB", 1 << 16);
}