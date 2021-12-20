#ifndef PRIME_H
#define PRIME_H

#include <cmath>
#include <cstdint>
#include <vector>

bool is_prime(const uint64_t);
bool is_prime(const uint64_t, std::vector<uint64_t> const &);
std::vector<uint64_t> generate_primes(const uint64_t);
std::vector<uint64_t> factor(uint64_t);

#endif // PRIME_H
