#include "ocalls.hpp"

#include <iostream>

using namespace std;

void ocall_say_hello_to(const char *who) {
  cout << "hello, "<<who<<endl;
}