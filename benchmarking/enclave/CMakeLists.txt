cmake_minimum_required(VERSION 3.14)

#set(enclaveLib ${enclave}.signed.so)

# must-linked libs
set(cLib sgx_tstdc)
set(cryptoLib sgx_tcrypto)
set(rtsLib sgx_trts${enclaveLibSuffix})
set(serviceLib sgx_tservice${enclaveLibSuffix})

# addon 
set(cxxLib sgx_tcxx)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${sgxPath}/include 
    ${sgxPath}/include/tlibc
    ${sgxPath}/include/libcxx
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${sgxFlags} \
    -nostdinc++ -fvisibility=hidden -fpie -ffunction-sections \
	-fdata-sections -fstack-protector-strong \
	-std=c++11")

# stronger security: -Wl,-z,relro,-z,now,-z,noexecstack
# be careful about the no-ASCII "-" when copying from dev-ref
set(LDFLAGS "-Wl,-pie,-eenclave_entry \
	-nostdlib -nodefaultlibs -nostartfiles -Wl,--no-undefined \
	-L${sgxLibPath} \
	-Wl,--whole-archive -l${rtsLib} -Wl,--no-whole-archive \
	-Wl,--start-group -l${cLib} -l${serviceLib} -l${cryptoLib} \
	-l${cxxLib} -Wl,--end-group \
	-Wl,-Bstatic -Wl,-Bsymbolic -Wl,--defsym=__ImageBase=0 -Wl,--export-dynamic")

set(bridgeImpl ${CMAKE_CURRENT_BINARY_DIR}/${enclave}_t.c) 
set(bridge ${CMAKE_CURRENT_BINARY_DIR}/${enclave}_t.h ${bridgeImpl})

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} srcs)
set(srcs "${srcs};${bridgeImpl}")

add_custom_command(
    OUTPUT ${bridge}
    COMMAND ${sgxEdger8r} --trusted ${edl} 
       --search-path ${enclavePath} --search-path ${sgxPath}/include
    COMMENT "[GEN] => ${bridge}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# build the raw lib
add_library(${enclave} SHARED ${srcs})

target_link_libraries(${enclave} ${LDFLAGS})

add_custom_command(TARGET ${enclave} POST_BUILD
    BYPRODUCTS ${enclaveLib} 
    COMMAND ${sgxSigner} sign -key ${key} -enclave lib${enclave}.so 
        -out ${enclaveLib} -config ${config}
    COMMENT "[SIGN] lib${enclave}.so => ${enclaveLib}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# the custom_command above can be replaced with below
#add_custom_target(${enclaveLib} ALL
#    ${sgxSigner} sign -key ${key} -enclave lib${enclave}.so 
#        -out ${enclaveLib} -config ${config}
#    BYPRODUCTS ${enclaveLib}
#    COMMENT "[SIGN] lib${enclave}.so => ${enclaveLib}"
#    DEPENDS ${enclave}
#    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

