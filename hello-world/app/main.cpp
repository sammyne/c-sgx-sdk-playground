#include <iostream>
#include <memory>
#include <vector>

#include "enclave_u.h"

#include "errors.hpp"
#include "loader.hpp"
#include "ocalls.hpp"

using namespace std;

using defer = shared_ptr<void>;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "missing enclave path :(" << endl;
        return -1;
    }
    auto enclave_path = argv[1];

    sgx_enclave_id_t enclave_id = 0;
    {
        sgx_status_t status;

        std::tie(enclave_id, status) = load_enclave(enclave_path);
        if (log_if_not_ok(status))
        {
            return -1;
        }
    }
    defer _1(nullptr, [&](...) { unload_enclave(enclave_id); });

    const char *user = "alice";

    auto err = SGX_SUCCESS;
    auto status = ecall_say_hello_to(enclave_id, &err, user);
    if (log_if_not_ok(status))
    {
        return -2;
    }
    else if (log_if_not_ok(err))
    {
        return -3;
    }

    vector<int> arr(8);
    for (size_t i = 0; i < arr.size(); i++)
    {
        arr[i] = i;
    }

    status = ecall_increment(enclave_id, arr.data(), arr.size());
    if (log_if_not_ok(status))
    {
        return -3;
    }

    for (size_t i = 0; i < arr.size(); i++)
    {
        cout << arr[i] << ",";
    }
    cout << endl;

    return 0;
}