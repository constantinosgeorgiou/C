#include <cstdio>
#include <vector>

using namespace std;

// still reachable block in valgrind output for vector of std threads
// https://stackoverflow.com/q/44643630/8049261

int main(int argc, char const *argv[]) {
  int n = 5;
  vector<int> numbers;
  for (int i = 0; i < n; i++) {
    numbers.push_back(i);
  }

  printf("Output of begin and end: ");
  for (auto i = numbers.begin(); i != numbers.end(); i++) printf("%d ", *i);
  printf("\n");

  printf("Output of cbegin and cend: ");
  for (auto i = numbers.cbegin(); i != numbers.cend(); i++) printf("%d ", *i);
  printf("\n");

  printf("Output of rbegin and rend: ");
  for (auto ir = numbers.rbegin(); ir != numbers.rend(); ir++)
    printf("%d ", *ir);
  printf("\n");

  printf("Output of crbegin and crend : ");
  for (auto ir = numbers.crbegin(); ir != numbers.crend(); ir++)
    printf("%d ", *ir);
  printf("\n");

  return 0;
}
