cmake_minimum_required(VERSION 3.14)

set(bridgeImpl ${CMAKE_CURRENT_BINARY_DIR}/${enclave}_u.c) 
set(bridge ${CMAKE_CURRENT_BINARY_DIR}/${enclave}_u.h ${bridgeImpl})

# must
set(rtsLib sgx_urts${enclaveLibSuffix})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${sgxFlags} \
    -fPIC -Wno-attributes ${headers} -Wnon-virtual-dtor -std=c++14 \
	-DDEBUG -UNDEBUG")

include_directories(${sgxPath}/include ${CMAKE_CURRENT_BINARY_DIR})

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} srcs)
set(srcs "${srcs};${bridgeImpl}")

add_custom_command(
    OUTPUT ${bridge}
    COMMAND ${sgxEdger8r} --untrusted ${edl} 
       --search-path ${enclavePath} --search-path ${sgxPath}/include
    COMMENT "[GEN] => ${bridge}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_executable(HelloWorld ${srcs})
target_link_libraries(HelloWorld 
    ${sgxFlags} 
	-L${sgxLibPath} 
	${rtsLib})

add_custom_target(dev ALL
    ${CMAKE_CURRENT_BINARY_DIR}/HelloWorld ${CMAKE_BINARY_DIR}/enclave/enclave.signed.so
    COMMENT "run the built binary"
    DEPENDS HelloWorld ${enclave})