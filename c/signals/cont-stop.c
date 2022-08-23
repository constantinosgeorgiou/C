#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>

int main(int argc, char const *argv[]) {
  int pid = fork();
  if (pid == -1) {
    return 1;
  }

  if (pid == 0) {
    while (1) {
      printf("Infinite loop\n");
      usleep(50000);
    }
  }

  kill(pid, SIGSTOP);

  int t;
  do {
    printf("Time in seconds for execution: ");
    scanf("%d", &t);

    if (t > 0) {
      kill(pid, SIGCONT);
      sleep(t);
      kill(pid, SIGSTOP);
    }
  } while (t > 0);

  kill(pid, SIGKILL);

  waitpid(pid, 0, 0);

  return 0;
}
