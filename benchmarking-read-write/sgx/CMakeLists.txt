cmake_minimum_required(VERSION 3.14)

set(program benchmarking-sgx-unprotected)

# following definition to enable benchmarking for Catch2
add_compile_definitions(
    CATCH_CONFIG_ENABLE_BENCHMARKING=1
)

# must
set(rtsLib sgx_urts${enclaveLibSuffix})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${sgxFlags} \
    -fPIC -Wno-attributes -Wnon-virtual-dtor -std=c++14 \
	-DDEBUG -UNDEBUG")

set(bridgeImpl ${CMAKE_CURRENT_BINARY_DIR}/enclave_u.c) 
set(bridge ${CMAKE_CURRENT_BINARY_DIR}/enclave_u.h ${bridgeImpl})

include_directories(
    ${sgxPath}/include
    ${utilsPath}
    ${catch2Path}
    ${CMAKE_CURRENT_BINARY_DIR}
)
 
aux_source_directory(${utilsPath} utilsSrcs)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} srcs)
set(srcs "${srcs};${utilsSrcs};${bridgeImpl}")

add_custom_command(
    OUTPUT ${bridge}
    COMMAND ${sgxEdger8r} --untrusted ${edl} 
       --search-path ${enclavePath} --search-path ${sgxPath}/include
    COMMENT "[GEN] => ${bridge}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
 
add_executable(${program} ${srcs})
target_link_libraries(${program} ${sgxFlags} -L${sgxLibPath} ${rtsLib})
 
add_custom_command(TARGET ${program}
    POST_BUILD
    COMMAND ./${program}
    COMMENT "run ${program}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${enclave})