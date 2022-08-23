#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void termination_handler(int signum) {
  printf("Hello from handler\n");
  sleep(1);
}

int main(void) {
  printf("Terminate using: 'killall sigact'\n");
  
  // Structs that will describe the old action and the new action
  // associated to the SIGINT signal (Ctrl+c from keyboard).
  struct sigaction new_action, old_action;

  // Set the handler in the new_action struct
  new_action.sa_handler = termination_handler;
  // Set to empty the sa_mask. It means that no signal is blocked
  //  while the handler run.
  sigemptyset(&new_action.sa_mask);
  // Block the SEGTERM signal.
  //  It means that while the handler run, the SIGTERM signal is ignored
  sigaddset(&new_action.sa_mask, SIGTERM);
  // Remove any flag from sa_flag. See documentation for flags allowed
  new_action.sa_flags = 0;

  // Read the old signal associated to SIGINT (keyboard, see signal(7))
  sigaction(SIGINT, NULL, &old_action);

  // If the old handler wasn't SIG_IGN (it's a handler that just
  //  "ignore" the signal)
  if (old_action.sa_handler != SIG_IGN) {
    // Replace the signal handler of SIGINT with the one described by new_action
    sigaction(SIGINT, &new_action, NULL);
  }

  while (1) {
    printf("In the loop\n");
    sleep(100);
  }
  return 0;
}
