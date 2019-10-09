#include "enclave_t.h"

void ecall_increment(int *arr, size_t arr_len)
{
    for (size_t i = 0; i < arr_len; i++)
    {
        arr[i]++;
    }
}

void ecall_read_protected(const int *arr, size_t arr_len)
{
    int buf = 0;
    for (size_t i = 0; i < arr_len; i++)
    {
        buf = arr[i];
    }

    auto _ = buf;
}

int ecall_read_unprotected(const int *arr, size_t arr_len)
{
    int buf = 0;
    for (size_t i = 0; i < arr_len; i++)
    {
        buf = arr[i];
    }

    return buf;
}