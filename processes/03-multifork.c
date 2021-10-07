#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char const *argv[]) {
  int id1 = fork();
  int id2 = fork();

  // This should be the result.
  // The boxes are processes.
  //
  // o---------o
  // | id1 = x |
  // | id2 = z |\
// o---------o\\
//     ||      \\
//     ||       \\
// o---------o  o---------o
  // | id1 = 0 |  | id1 = x |
  // | id2 = y |  | id2 = 0 |
  // o---------o  o---------o
  //     ||
  //     ||
  // o---------o
  // | id1 = 0 |
  // | id2 = 0 |
  // o---------o

  if (id1 == 0) {

    if (id2 == 0)
      printf("We are process y\n");
    else
      printf("We are process x\n");
  } else {
    if (id2 == 0)
      printf("We are process z\n");
    else
      printf("We are the parent process!\n");
  }

  while (wait(NULL) != -1 || errno != ECHILD) {
    printf("Waited for a child to finish\n");
  }
  return 0;
}
