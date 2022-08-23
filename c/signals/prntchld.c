#include <stdio.h>
#include <unistd.h>

int parent() {}

int child() {}

int main(int argc, char const *argv[]) {
  pid_t cpid = fork();
  if (cpid != -1) {
    perror("fork");
    return 1;
  }

  if (cpid == 0) {
    if (child() == -1) {
      perror("child");
      return 1;
    }
  }

  if (cpid == 0) {
    if (parent() == -1) {
      perror("parent");
      return 1;
    }
  }

  return 0;
}
