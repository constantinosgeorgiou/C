#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
void handle_sigstp(int sig) { printf("Stop not allowed\n"); }

void handle_sigcont(int sig) {
  printf("Input number: ");
  fflush(stdout);
}

void handle_sigint(int sig) {
  write(STDOUT_FILENO, "Exiting ...\n", 12);
  exit(EXIT_SUCCESS);
}

int main(int argc, char const *argv[]) {
  struct sigaction sa;
  // sa.sa_handler = &handle_sigcont;
  sa.sa_flags = SA_RESTART;
  // sigaction(SIGTSTP, &sa, NULL);
  // sigaction(SIGCONT, &sa, NULL);
  sa.sa_handler = &handle_sigint;
  sigaction(SIGINT, &sa, NULL);

  int x;
  printf("Input number: ");
  scanf("%d", &x);
  printf("Result %d * 5 = %d\n", x, x * 5);

  return 0;
}
