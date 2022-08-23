#include <cstdio>
#include <map>
#include <string>

using namespace std;

bool test_exist(map<string, int> pmap = {}) {
  if (!pmap.empty()) {
    return true;
  }
  return false;
}

int main(int argc, char const *argv[]) {
  map<string, int> fruits;
  printf("Fruits so far: %ld\n", fruits.size());

  fruits.insert(pair<string, int>("apple", 1));
  printf("Fruits so far: %ld\n", fruits.size());
  printf("Apples: %d\n", fruits["apple"]);
  fruits["apple"]++;
  printf("Apples: %d\n", fruits["apple"]);

  fruits.insert({"pineapple", 5});
  printf("Fruits so far: %ld\n", fruits.size());
  int count = fruits["pineapple"];
  count++;
  printf("Pineapples: %d\n", count);
  fruits["pineapple"] = count;
  printf("Pineapples now: %d\n", fruits["pineapple"]);

  fruits["banana"] = 2;
  fruits["coconut"] = 10;

  int maybe;
  maybe = fruits["kiwi"];
  // maybe = fruits.at("berry");

  fruits["orange"]++;

  printf("key/value pairs:\n");
  for (auto fruit : fruits) {
    printf(" (%s, %d)\n", fruit.first.c_str(), fruit.second);
  }

  bool res1 = test_exist();
  bool res2 = test_exist(fruits);
  printf("%d    %d\n", res1, res2);

  return 0;
}
