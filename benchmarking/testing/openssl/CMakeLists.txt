cmake_minimum_required(VERSION 3.14)

#project(openssl-benchmarking)

find_package(OpenSSL REQUIRED)

include_directories(${OPENSSL_INCLUDE_DIR})

# following definition to enable benchmarking for Catch2
add_compile_definitions(
    CATCH_CONFIG_ENABLE_BENCHMARKING=1
)

add_subdirectory(aes)
add_subdirectory(sha256)