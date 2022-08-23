#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>

int x = 0;

void handle_sigurs1(int sig) {
  if (x == 0) {
    printf("\nHINT: Remember multiplication is repetitive addition!\n");
  }
}

int main(int argc, char const *argv[]) {
  pid_t cpid = fork();
  if (cpid == -1) {
    return 1;
  }

  if (cpid == 0) {
    sleep(5);
    kill(getppid(), SIGUSR1);
  }

  struct sigaction sa = {0};
  sa.sa_flags = SA_RESTART;
  sa.sa_handler = &handle_sigurs1;
  sigaction(SIGUSR1, &sa, NULL);

  printf("What is the result of 3x5: ");
  scanf("%d", &x);
  if (x == 15) {
    printf("Right!");

  } else {
    printf("Wrong!");
  }
  waitpid(cpid, 0, 0);

  return 0;
}
