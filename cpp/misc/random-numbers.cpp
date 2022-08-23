#include <cctype>
#include <iostream>
#include <random>

using u32 = uint_least32_t;
using engine = std::mt19937;

int main(void) {
  std::random_device os_seed;
  const u32 seed = os_seed();

  engine generator(seed);
  std::uniform_int_distribution<u32> distribute(0, 5);

  for (int repetition = 0; repetition < 10; ++repetition)
    std::cout << distribute(generator) << std::endl;
  return 0;
}