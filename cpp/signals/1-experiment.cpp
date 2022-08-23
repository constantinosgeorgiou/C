// #include <csignal>
#include <signal.h>

#include <cstdio>

static void handle_termination(int sig) {}

int main(int argc, char const *argv[]) {
  struct sigaction action = {0};
  action.__sigaction_handler = handle_termination;
  int status = sigaction();

  return 0;
}
