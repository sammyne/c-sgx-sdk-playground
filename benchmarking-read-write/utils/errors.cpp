#include "errors.hpp"

#include <stdio.h>
#include <unistd.h>

#include "sgx_urts.h"

typedef struct _sgx_errlist_t
{
  sgx_status_t err;
  const char *msg;
  const char *sug; /* Suggestion */
} sgx_errlist_t;

static sgx_errlist_t sgx_errlist[] = {
    {SGX_ERROR_UNEXPECTED,
     "Unexpected error occurred.",
     NULL},
    {SGX_ERROR_INVALID_PARAMETER,
     "Invalid parameter.",
     NULL},
    {SGX_ERROR_OUT_OF_MEMORY,
     "Out of memory.",
     NULL},
    {SGX_ERROR_ENCLAVE_LOST,
     "Power transition occurred.",
     "Please refer to the sample \"PowerTransition\" for details."},
    {SGX_ERROR_INVALID_ENCLAVE,
     "Invalid enclave image.",
     NULL},
    {SGX_ERROR_INVALID_ENCLAVE_ID,
     "Invalid enclave identification.",
     NULL},
    {SGX_ERROR_INVALID_SIGNATURE,
     "Invalid enclave signature.",
     NULL},
    {SGX_ERROR_OUT_OF_EPC,
     "Out of EPC memory.",
     NULL},
    {SGX_ERROR_NO_DEVICE,
     "Invalid SGX device.",
     "Please make sure SGX module is enabled in the BIOS, and install SGX driver afterwards."},
    {SGX_ERROR_MEMORY_MAP_CONFLICT,
     "Memory map conflicted.",
     NULL},
    {SGX_ERROR_INVALID_METADATA,
     "Invalid enclave metadata.",
     NULL},
    {SGX_ERROR_DEVICE_BUSY,
     "SGX device was busy.",
     NULL},
    {SGX_ERROR_INVALID_VERSION,
     "Enclave version was invalid.",
     NULL},
    {SGX_ERROR_INVALID_ATTRIBUTE,
     "Enclave was not authorized.",
     NULL},
    {SGX_ERROR_ENCLAVE_FILE_ACCESS,
     "Can't open enclave file.",
     NULL},
};

/* Check error conditions for loading enclave */
void print_error_message(sgx_status_t ret)
{
  size_t idx = 0;
  size_t ttl = sizeof sgx_errlist / sizeof sgx_errlist[0];

  for (idx = 0; idx < ttl; idx++)
  {
    if (ret == sgx_errlist[idx].err)
    {
      if (NULL != sgx_errlist[idx].sug)
        printf("Info 0x%X: %s\n", ret, sgx_errlist[idx].sug);
      printf("Error 0x%X: %s\n", ret, sgx_errlist[idx].msg);
      break;
    }
  }

  if (idx == ttl)
    printf("Error code is 0x%X. Please refer to the \"Intel SGX SDK Developer Reference\" for more details.\n", ret);
}

bool log_if_not_ok(sgx_status_t err)
{
  auto notOk = (SGX_SUCCESS != err);
  if (notOk)
  {
    print_error_message(err);
  }

  return notOk;
}