#include "enclave_t.h"

void ecall_increment(int *arr, size_t arr_len)
{
    for (size_t i = 0; i < arr_len; i++)
    {
        arr[i]++;
    }
}

sgx_status_t ecall_say_hello_to(const char *who) {
    return ocall_say_hello_to(who);
}