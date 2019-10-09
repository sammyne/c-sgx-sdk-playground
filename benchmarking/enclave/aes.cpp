#include "enclave_t.h"

sgx_status_t ecall_aes128_gcm_encrypt(
    uint8_t *ciphertext, uint8_t *tag,
    const uint8_t *key, const uint8_t *msg, size_t msg_len, const uint8_t *iv, size_t iv_len)
{
    auto key_ = (sgx_aes_gcm_128bit_key_t *)(&key);
    auto tag_ = (sgx_aes_gcm_128bit_tag_t *)(&tag);
    return sgx_rijndael128GCM_encrypt(key_, msg, msg_len, ciphertext, iv, iv_len, nullptr, 0, tag_);
}