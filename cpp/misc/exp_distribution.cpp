// exponential_distribution
#include <iostream>
#include <random>

int main() {
  const int nrolls = 100;     // number of experiments
  const int nstars = 10;      // maximum number of stars to distribute
  const int nintervals = 10;  // number of intervals

  std::default_random_engine generator;
  std::exponential_distribution<double> distribution(3.5);

  int p[nintervals] = {};

  for (int i = 0; i < nrolls; ++i) {
    double number = distribution(generator);
    std::cout << 10 * number << std::endl;
    // if (number < 1.0) ++p[int(nintervals * number)];
  }

  // std::cout << "exponential_distribution (3.5):" << std::endl;
  // std::cout << std::fixed;
  // std::cout.precision(1);

  // for (int i = 0; i < nintervals; ++i) {
  //   std::cout << p[i] << std::endl;
  // }

  // for (int i = 0; i < nintervals; ++i) {
  //   std::cout << float(i) / nintervals << "-" << float(i + 1) / nintervals
  //             << ": ";
  //   std::cout << std::string(p[i] * nstars / nrolls, '*') << std::endl;
  // }

  return 0;
}