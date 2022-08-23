#include <cstdio>
#include <queue>
#include <string>

using namespace std;

int main(int argc, char const *argv[]) {
  queue<string> q;
  string front = "";

  q.push("first");
  front = q.front();
  printf("front: (%ld,%ld) %s\n", front.length(), front.size(), front.c_str());

  q.push("second");
  front = q.front();
  printf("front: (%ld,%ld) %s\n", front.length(), front.size(), front.c_str());

  q.pop();
  front = q.front();
  printf("front: (%ld,%ld) %s\n", front.length(), front.size(), front.c_str());

  q.pop();
  front = q.front();
  printf("front: (%ld,%ld) %s\n", front.length(), front.size(), front.c_str());

  return 0;
}
