#pragma once

#include <climits>
#include <iostream>
#include <tuple>

#include "sgx_urts.h"

const sgx_enclave_id_t ErrEnclaveID = INT_MAX;

std::tuple<sgx_enclave_id_t, sgx_status_t> load_enclave(const std::string path);

//sgx_status_t quitRA(sgx_enclave_id_t enclaveID, sgx_ra_context_t ctx);
sgx_status_t unload_enclave(const sgx_enclave_id_t ID);
