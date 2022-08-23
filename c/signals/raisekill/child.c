#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static volatile sig_atomic_t keep_running = 1;

void handler(int signum) {
  if (signum == SIGINT || signum == SIGTERM) {
    keep_running = 0;
  }
}

int main(int argc, char const *argv[]) {
  // Using the exec function, any signals that you’ve
  // defined your own handlers for revert to their SIG_DFL handling.
  struct sigaction action, query_action;

  action.sa_handler = &handler;

  // Do not block any signals.
  if (sigfillset(&action.sa_mask) == -1) {
    perror("parent-sigfillset");
    return 1;
  }

  action.sa_flags =
      SA_RESTART;  // When a signal is delivered during certain primitives (such
                   // as open, read or write), returning from a handler resumes
                   // the library function.

  sigaction(SIGINT, NULL, &query_action);
  if (query_action.sa_handler != SIG_IGN) {
    if (sigaction(SIGINT, &action, NULL) == -1) {
      perror("child-sigaction-SIGINT");
      return 1;
    }
  }
  sigaction(SIGTERM, NULL, &query_action);
  if (query_action.sa_handler != SIG_IGN) {
    if (sigaction(SIGTERM, &action, NULL) == -1) {
      perror("child-sigaction-SIGTERM");
      return 1;
    }
  }

  while (keep_running) {
    printf("[Child:%d] Stopping self ...\n", getpid());
    sleep(1);
    if (raise(SIGSTOP) == -1) {
      perror("child-raise");
      return 1;
    }

    printf("[Child:%d] Input text: ", getpid());
    fflush(stdout);
    char text[BUFSIZ];
    scanf("%s", text);

    printf("[Child:%d] Received text '%s'\n", getpid(), text);

    memset(text, 0x00, BUFSIZ);
  }

  printf("[Child:%d] Exiting ...", getpid());

  return 0;
}
