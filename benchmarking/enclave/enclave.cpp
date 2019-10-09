#include "enclave_t.h"

#include "sgx_tcrypto.h"

sgx_status_t ecall_sha256(sgx_sha256_hash_t *hash, const uint8_t *msg, size_t msg_len)
{
    return sgx_sha256_msg(msg, msg_len, hash);
}
