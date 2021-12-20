# C++ primality test, prime number generation, and factorization (elementary)

I have been re-learning programming lately using C++.

After Hello World, I wrote a simple program about prime numbers.

## Primality test

A natural number n > 1 is prime if and only if it is not divisible by any natural number i such that 2 <= i <= n - 1. The numbers 0 and 1 are not prime.

A simple C++ program to print the primes less than or equal to 100 would be as follows:

```cpp
#include <iostream>

// Returns whether `n` is prime.
bool is_prime(int n) {
  if (n <= 1) {
    return false;
  }

  for (int i = 2; i < n; i++) {
    if (n % i == 0) {
      return false;
    }
  }

  return true;
}

int main() {
  // Prints the prime numbers less than or equal to 100.
  for (int n = 0; n <= 100; n++) {
    if (is_prime(n)) {
      std::cout << n << std::endl;
    }
  }
}
```

```
⋊> g++ -Ofast -std=c++2a -Wall -Wextra -Werror -pedantic-errors -o main main.cc; and time ./main | wc -l
      25

________________________________________________________
Executed in    5.37 millis    fish           external
   usr time    2.70 millis  159.00 micros    2.54 millis
   sys time    4.52 millis  834.00 micros    3.69 millis
```

This algorithm is called trial division, which attempts to divide n by smaller numbers.

There are some improvements that can be made to this program:

- Using `using namespace std`, it is possible to avoid writing `std::` every time like `std::cout` or `std::endl`. This is considered bad in header files (with extensions `*.h`, `*.hpp`, etc.), but not in source files (with extensions `*.cc`, `*.cpp`, etc.).
- Using `const` for constants, such as `is_prime`'s `n` argument, is a good practice, just like in JavaScript/TypeScript.
- `i++` should be written as `++i`. They are the same for primitives such as `int`, but the latter is sometimes faster in other cases. For consistency, the latter form should always be used.
- `unsigned int` (aka `unsigned`) should be used instead of `int`.<br>According to [Wikipedia](https://en.wikipedia.org/wiki/C_data_types#Main_types), `int` can handle integers in the range [-32767, +32767], while `unsigned` can handle [0, 65535].<br>It is controversial whether we should always use `unsigned` for non-negative integers such as loop counters, but in this case, we will want to handle as large an integer as possible, so we should definitely use it.
	- Or better yet, `unsigned long long int` (aka `unsigned long long`) should be used, which is guaranteed to handle [0, 2^64 - 1]. It is given the short name `uint64_t` in `<cstdint>`.
- For `main`:
	- For reusability, `main` should be factored out to "a function that returns a vector of primes less than or equal to a given natural number".
	- It should only iterate over odd numbers, since all primes except 2 are odd.
- For `is_prime`:
	- It is enough to trial divide by only the primes in [2, n - 1]. Because if n is divisible by a composite number, then it must already be divisible by its prime factor.<br>We can emulate it by iterating over the odd numbers in that range, if n is not even.
	- It is enough to trial divide only in [2, sqrt(2)], as is well known.<br>I don't know how to prove it, but I think it can be demonstrated by the following example:<br>Consider whether 113 is prime. It is not divisible by 2, 3, 5, or 7. Then there is no need to divide by 11. Because if 113 = 11 * x, then x < 11, so it must already divisible by a smaller prime. (Hence, 113 is prime.)

Here is the program with these improvements:

```cpp
#include <cmath>   // for std::sqrt
#include <cstdint> // for std::uint64_t
#include <iostream>
#include <vector>

using namespace std;

// Returns whether `n` is prime.
bool is_prime(const uint64_t n) {
  if (n <= 2) {
    return n == 2;
  }

  if (n % 2 == 0) {
    return false;
  }

  const uint64_t sqrt_n = sqrt(n);
  for (uint64_t i = 3; i <= sqrt_n; i += 2) {
    if (n % i == 0) {
      return false;
    }
  }

  return true;
}

// Returns a vector of prime numbers less than or equal to `max_num`.
vector<uint64_t> generate_primes(const uint64_t max_num) {
  if (max_num <= 1) {
    return {};
  }

  vector<uint64_t> primes = {2};

  for (uint64_t n = 3; n <= max_num; n += 2) {
    if (is_prime(n)) {
      primes.push_back(n);
    }
  }

  return primes;
}

int main() {
  // Prints the prime numbers less than or equal to 10 million.
  for (auto p : generate_primes(10000000)) {
    cout << p << endl;
  }
}
```

```
⋊> g++ -Ofast -std=c++2a -Wall -Wextra -Werror -pedantic-errors -o main main.cc; and time ./main | wc -l
  664579

________________________________________________________
Executed in    4.78 secs   fish           external
   usr time    4.11 secs  183.00 micros    4.11 secs
   sys time    1.22 secs  879.00 micros    1.22 secs
```

In `is_prime`, the cryptic

```cpp
if (i <= 2) {
  return n == 2;
}
```

can be written more intuitively as

```cpp
if (i <= 1) {
  return false;
}

if (i == 2) {
  return true;
}
```

but I think the former is preferred to reduce the number of comparisons when both conditions do not hold (which is most of the time), but I'm not sure.

Also, `i <= sqrt_n` can also be written as `i * i <= n`, but then `i * i` will be calculated in each loop and may overflow.

By writing `i <= n / i`, it is possible to avoid the overflow, but `n / i` will still be calculated in each loop.

Further optimization is possible.

According to various sources, it appears that primes greater than or equal to 5 are of the form 6k +- 1.

Therefore, after checking that n is not a multiple of 2 or 3, it is enough to trial divide by only numbers of that form, instead of odd numbers, as "possible primes".

Below is the program with that optimization:

```cpp
#include <cmath>   // for std::sqrt
#include <cstdint> // for std::uint64_t
#include <iostream>
#include <vector>

using namespace std;

// Returns whether `n` is prime.
bool is_prime(const uint64_t n) {
  if (n <= 3) {
    return n > 1;
  }

  if (n % 2 == 0 || n % 3 == 0) {
    return false;
  }

  const uint64_t sqrt_n = sqrt(n);
  // Divides by integers of the form 6k +- 1.
  for (uint64_t i = 5; i <= sqrt_n; i += 6) {
    if (n % i == 0 || n % (i + 2) == 0) {
      return false;
    }
  }

  return true;
}

// Returns a vector of prime numbers less than or equal to `max_num`.
vector<uint64_t> generate_primes(const uint64_t max_num) {
  if (max_num <= 2) {
    return max_num <= 1 ? vector<uint64_t>{} : vector<uint64_t>{2};
  }

  vector<uint64_t> primes = {2, 3};

  // Iterates over integers of the form 6k +- 1.
  for (uint64_t n = 5; n <= max_num; n += 4) {
    if (is_prime(n)) {
      primes.push_back(n);
    }

    n += 2;

    if (n > max_num) {
      break;
    }

    if (is_prime(n)) {
      primes.push_back(n);
    }
  }

  return primes;
}

int main() {
  // Prints the prime numbers less than or equal to 10 million.
  for (auto p : generate_primes(10000000)) {
    cout << p << endl;
  }
}
```

```
⋊> g++ -Ofast -std=c++2a -Wall -Wextra -Werror -pedantic-errors -o main main.cc; and time ./main | wc -l
  664579

________________________________________________________
Executed in    3.64 secs   fish           external
   usr time    3.08 secs  178.00 micros    3.08 secs
   sys time    1.35 secs  900.00 micros    1.35 secs
```

When printing primes starting from 2, it is actually possible to "divide by primes" since the smaller primes are found gradually.

Below is the version with the two-argument `is_prime` that trial divides by the elements of the given vector `primes`:

```cpp
#include <cmath>   // for std::sqrt
#include <cstdint> // for std::uint64_t
#include <iostream>
#include <vector>

using namespace std;

// Returns whether `n` is prime. Determined by trial division by `primes`.
bool is_prime(const uint64_t n, vector<uint64_t> const &primes) {
  if (n <= 3) {
    return n > 1;
  }

  const uint64_t sqrt_n = sqrt(n);
  for (uint64_t i = 0; primes[i] <= sqrt_n; ++i) {
    if (n % primes[i] == 0) {
      return false;
    }
  }

  return true;
}

// Returns a vector of prime numbers less than or equal to `max_num`.
vector<uint64_t> generate_primes(const uint64_t max_num) {
  if (max_num <= 2) {
    return max_num <= 1 ? vector<uint64_t>{} : vector<uint64_t>{2};
  }

  vector<uint64_t> primes = {2, 3};

  // Iterates over integers of the form 6k +- 1.
  for (uint64_t n = 5; n <= max_num; n += 4) {
    if (is_prime(n, primes)) {
      primes.push_back(n);
    }

    n += 2;

    if (n > max_num) {
      break;
    }

    if (is_prime(n, primes)) {
      primes.push_back(n);
    }
  }

  return primes;
}

int main() {
  // Prints the prime numbers less than or equal to 10 million.
  for (auto p : generate_primes(10000000)) {
    cout << p << endl;
  }
}
```

```
⋊> g++ -Ofast -std=c++2a -Wall -Wextra -Werror -pedantic-errors -o main main.cc; and time ./main | wc -l
  664579

________________________________________________________
Executed in    2.68 secs   fish           external
   usr time    1.96 secs  181.00 micros    1.96 secs
   sys time    1.16 secs  827.00 micros    1.16 secs
```

In C++, it appears that vectors are passed by reference if `&` is added to the type of the function argument. If not, they are passed by value, which can take a lot of time.

For the problem of "determining whether a number is prime", the code described above is fine (at an elementary level).

However, for the problem of "enumerating the primes less than or equal to a given number", the most standard way is apparently not trial division, but the famous sieve of Eratosthenes.

## Prime number generation

I had heard of the sieve of Eratosthenes, but never implemented it before.

As far as I can tell from [Wikipedia](https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes), the simplest implementation of the sieve of Eratosthenes would be as follows:

```cpp
#include <iostream>
#include <vector>

using namespace std;

// Returns a vector of prime numbers less than or equal to `n`. Generated by the
// sieve of Eratosthenes.
vector<int> generate_primes(int n) {
  vector<bool> primality(n + 1, true);

  for (int i = 2; i <= n; ++i) {
    if (primality[i]) {
      for (int j = i + i; j <= n; j += i) {
        primality[j] = false;
      }
    }
  }

  vector<int> primes = {};

  for (int i = 2; i <= n; ++i) {
    if (primality[i]) {
      primes.push_back(i);
    }
  }

  return primes;
}

int main() {
  // Prints the prime numbers less than or equal to 10 million.
  for (auto p : generate_primes(10000000)) {
    cout << p << endl;
  }
}
```

```
⋊> g++ -Ofast -std=c++2a -Wall -Wextra -Werror -pedantic-errors -o main main.cc; and time ./main | wc -l
  664579

________________________________________________________
Executed in    1.70 secs   fish           external
   usr time    1.07 secs  206.00 micros    1.07 secs
   sys time    1.34 secs  1002.00 micros    1.34 secs
```

There is an obvious waste in this code. For example, `primes.push_back(i)` can be done in the preceding `for` loop (but that won't work if `i <= n` is changed to `i <= sqrt(n)`).

The code with the optimizations described in Wikipedia and the 6k +- 1 optimization described above is as follows:

```cpp
#include <cmath>   // for std::sqrt
#include <cstdint> // for std::uint64_t
#include <iostream>
#include <vector>

using namespace std;

// Returns a vector of prime numbers less than or equal to `n`. Generated by the
// sieve of Eratosthenes.
vector<uint64_t> generate_primes(const uint64_t n) {
  if (n <= 2) {
    return n <= 1 ? vector<uint64_t>{} : vector<uint64_t>{2};
  }

  vector<uint64_t> primes = {2, 3};
  vector<bool> primality(n + 1, true);

  const uint64_t sqrt_n = sqrt(n);
  uint64_t i;
  // Iterates over integers of the form 6k +- 1.
  for (i = 5; i <= sqrt_n; i += 4) {
    if (primality[i]) {
      primes.push_back(i);

      for (uint64_t j = i * i; j <= n; j += i) {
        primality[j] = false;
      }
    }

    i += 2;

    if (primality[i]) {
      primes.push_back(i);

      for (uint64_t j = i * i; j <= n; j += i) {
        primality[j] = false;
      }
    }
  }

  for (; i <= n; i += 4) {
    if (primality[i]) {
      primes.push_back(i);
    }

    i += 2;

    if (i > n) {
      break;
    }

    if (primality[i]) {
      primes.push_back(i);
    }
  }

  return primes;
}

int main() {
  // Prints the prime numbers less than or equal to 10 million.
  for (auto p : generate_primes(10000000)) {
    cout << p << endl;
  }
}
```

```
⋊> g++ -Ofast -std=c++2a -Wall -Wextra -Werror -pedantic-errors -o main main.cc; and time ./main | wc -l
  664579

________________________________________________________
Executed in    1.44 secs   fish           external
   usr time    0.93 secs  172.00 micros    0.93 secs
   sys time    1.15 secs  645.00 micros    1.15 secs
```

## Prime factorization

The simplest code would be as follows:

```cpp
#include <iostream>
#include <vector>

using namespace std;

// Returns a vector of prime factors of `n`.
vector<int> factor(int n) {
  vector<int> prime_factors;

  for (int i = 2; i < n; ++i) {
    while (n % i == 0) {
      prime_factors.push_back(i);
      n /= i;
    }
  }

  if (n != 1) {
    prime_factors.push_back(n);
  }

  return prime_factors;
}

int main() {
  // Prints the prime factorization of the natural numbers from 2 to 10.
  for (int n = 2; n <= 10; ++n) {
    cout << n;
    for (auto p : factor(n)) {
      cout << "\t" << p;
    }
    cout << endl;
  }
}
```

```
⋊> g++ -Ofast -std=c++2a -Wall -Wextra -Werror -pedantic-errors -o main main.cc; and time ./main
2       2
3       3
4       2       2
5       5
6       2       3
7       7
8       2       2       2
9       3       3
10      2       5

________________________________________________________
Executed in    6.52 millis    fish           external
   usr time    2.35 millis  126.00 micros    2.23 millis
   sys time    2.68 millis  627.00 micros    2.06 millis
```

And here is the 6k +- 1 optimized code that prints the prime factorization of the decimal repunits (natural numbers of the form 111...111) up to the 20th.

```cpp
#include <cmath>   // for std::sqrt
#include <cstdint> // for std::uint64_t
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Returns a vector of prime factors of `n`.
vector<uint64_t> factor(uint64_t n) {
  if (n <= 3) {
    return n == 1 ? vector<uint64_t>{} : vector<uint64_t>{n};
  }

  vector<uint64_t> prime_factors;

  while (n % 2 == 0) {
    prime_factors.push_back(2);
    n /= 2;
  }

  while (n % 3 == 0) {
    prime_factors.push_back(3);
    n /= 3;
  }

  const uint64_t sqrt_n = sqrt(n);
  // Divides by integers of the form 6k +- 1.
  for (uint64_t i = 5; i <= sqrt_n; i += 4) {
    while (n % i == 0) {
      prime_factors.push_back(i);
      n /= i;
    }

    i += 2;

    while (n % i == 0) {
      prime_factors.push_back(i);
      n /= i;
    }
  }

  if (n != 1) {
    prime_factors.push_back(n);
  }

  return prime_factors;
}

int main() {
  // Prints the prime factorization of the decimal repunits up to the 20th.
  for (int i = 2; i <= 20; ++i) {
    const uint64_t repunit = stoull(string(i, '1'));

    cout << i << "\t" << repunit;
    for (auto p : factor(repunit)) {
      cout << "\t" << p;
    }
    cout << endl;
  }
}
```

```
⋊> g++ -Ofast -std=c++2a -Wall -Wextra -Werror -pedantic-errors -o main main.cc; and time ./main
2       11      11
3       111     3       37
4       1111    11      101
5       11111   41      271
6       111111  3       7       11      13      37
7       1111111 239     4649
8       11111111        11      73      101     137
9       111111111       3       3       37      333667
10      1111111111      11      41      271     9091
11      11111111111     21649   513239
12      111111111111    3       7       11      13      37      101     9901
13      1111111111111   53      79      265371653
14      11111111111111  11      239     4649    909091
15      111111111111111 3       31      37      41      271     2906161
16      1111111111111111        11      17      73      101     137     5882353
17      11111111111111111       2071723 5363222357
18      111111111111111111      3       3       7       11      13      19      37      52579   333667
19      1111111111111111111     1111111111111111111
20      11111111111111111111    11      41      101     271     3541    9091    27961

________________________________________________________
Executed in   16.24 secs   fish           external
   usr time   15.01 secs  178.00 micros   15.01 secs
   sys time    0.18 secs  1066.00 micros    0.18 secs
```

Since R(21) = (10^21 - 1)/9 > 2^64 - 1, factoring more repunits will cause an overflow in this code.
