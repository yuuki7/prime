// Prints the prime numbers less than or equal to 10 million.
#include "prime.h"
#include <iostream>

using namespace std;

int main() {
  for (auto p : generate_primes(10000000)) {
    cout << p << endl;
  }
}
