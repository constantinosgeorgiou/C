#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int child(void) {
  execlp("echo", "echo", "Hello from child!", (char *)NULL);
  perror("execlp");
  return -1;
}

int main(int argc, char const *argv[]) {
  pid_t cpid = fork();
  if (cpid == -1) {
    perror("fork");
    return 1;
  }

  if (cpid == 0) {
    if (child() == -1) {
      perror("child");
      return 1;
    }
  }

  printf("Waiting for child %d\n", cpid);
  waitpid(cpid, 0, 0);

  return 0;
}
