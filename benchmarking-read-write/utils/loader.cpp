#include "loader.hpp"

#include "enclave_u.h"

using namespace std;

tuple<sgx_enclave_id_t, sgx_status_t> load_enclave(const string path)
{
    sgx_enclave_id_t enclaveID = 0;

    /* [deprecated] Step 1: try to retrieve the launch token saved by last transaction 
     *         if there is no token, then create a new one.
     */

    /* Step 2: call sgx_create_enclave to initialize an enclave instance */
    /* Debug Support: set 2nd parameter to 1 */

    // token and updated is deprecated @see dev-ref p131
    sgx_launch_token_t token = {0x00};
    int updated = 0;
    auto ret = sgx_create_enclave(path.c_str(), SGX_DEBUG_FLAG,
                                  &token, &updated, &enclaveID, NULL);
    if (ret != SGX_SUCCESS)
    {
        //print_error_message(ret);
        return make_tuple(ErrEnclaveID, ret);
    }

    /* [deprecated] Step 3: save the launch token if it is updated */

    return make_tuple(enclaveID, SGX_SUCCESS);
}

sgx_status_t unload_enclave(const sgx_enclave_id_t ID)
{
    return sgx_destroy_enclave(ID);
}