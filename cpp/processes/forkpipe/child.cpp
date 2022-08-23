#include <sys/wait.h>
#include <unistd.h>

#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <queue>
#include <random>
#include <string>

using namespace std;

static volatile sig_atomic_t keep_running = true;

string random_string(size_t length) {
  const string CHARACTERS =
      "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

  random_device random_device;
  mt19937 generator(random_device());
  uniform_int_distribution<> distribution(0, CHARACTERS.size() - 1);

  string random_string;

  for (size_t i = 0; i < length; ++i) {
    random_string += CHARACTERS[distribution(generator)];
  }

  return random_string;
}

int main(int argc, char const *argv[]) {
  // while (keep_running) {
    // Pipe strings over to parent.
    string rstring = random_string(10);
    printf("%s\n", rstring.c_str());
  // }

  return 0;
}
