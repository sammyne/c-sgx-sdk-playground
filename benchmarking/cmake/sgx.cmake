cmake_minimum_required(VERSION 3.14)

if(CMAKE_BUILD_TYPE MATCHES "Debug" OR CMAKE_BUILD_TYPE MATCHES "")
   set(sgxFlags "-m64 -O0 -g")
   set(enclaveLibSuffix _sim)
elseif(CMAKE_BUILD_TYPE MATCHES "Prerelease")
   set(sgxFlags "-g -O2")
elseif(CMAKE_BUILD_TYPE MATCHES "Release")
   set(sgxFlags "-g -O2")
else()
   message(FATAL_ERROR "unknown build type: ${CMAKE_BUILD_TYPE}")
endif()

set(sgxPath /opt/intel/sgxsdk)
set(sgxLibPath ${sgxPath}/lib64)

# tools
set(sgxEdger8r ${sgxPath}/bin/x64/sgx_edger8r)
set(sgxSigner ${sgxPath}/bin/x64/sgx_sign)

set(appPath ${CMAKE_CURRENT_SOURCE_DIR}/app)
set(enclavePath ${CMAKE_CURRENT_SOURCE_DIR}/enclave)

set(enclave enclave)

set(config ${enclavePath}/${enclave}.config.xml)
set(edl enclave.edl)
set(enclaveLib ${enclave}.signed.so)
set(key ${enclavePath}/isv.pem)