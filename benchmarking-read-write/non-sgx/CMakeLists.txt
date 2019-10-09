cmake_minimum_required(VERSION 3.14)

set(program non-sgx)

# following definition to enable benchmarking for Catch2
add_compile_definitions(
    CATCH_CONFIG_ENABLE_BENCHMARKING=1
)

include_directories(${catch2Path})

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} srcs)

add_executable(benchmarking-${program} ${srcs})

add_custom_command(TARGET benchmarking-${program}
    POST_BUILD
    COMMAND ./benchmarking-${program}
    COMMENT "benchmarking ${program}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})