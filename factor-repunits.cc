// Prints the prime factorization of the decimal repunits up to the 20th.
#include "prime.h"
#include <cstdint>
#include <iostream>
#include <string>

using namespace std;

int main() {
  for (int i = 2; i <= 20; ++i) {
    const uint64_t repunit = stoull(string(i, '1'));

    cout << i << "\t" << repunit;
    for (auto p : factor(repunit)) {
      cout << "\t" << p;
    }
    cout << endl;
  }
}
