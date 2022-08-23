#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define CHILDREN 1

static volatile sig_atomic_t keep_running = 1;
static volatile sig_atomic_t stopped_child = 0;

void handler(int signum) {
  if (signum == SIGINT) {
    keep_running = 0;
  }
  if (signum == SIGCHLD) {
    stopped_child = 1;
  }
}

int collect(pid_t child) {
  printf("[Parent] Collecting ... \n");

  int collected = 0;
  pid_t pid;
  int status;
  //  To avoid this calls to waitpid should be protected using cancellation
  //  handlers.

  while ((pid = waitpid(WAIT_ANY, &status, WNOHANG | WUNTRACED)) != 0) {
    if (pid == -1) {
      perror("parent-collect-waitpid");
      return -1;
    }
    printf("          - Child %d with status %d\n", pid, status);
    collected++;
  }

  return collected;
}

int main(int argc, char const *argv[]) {
  struct sigaction action, query_action;
  sigset_t allow_all_mask;

  // Do not block any signals.
  if (sigfillset(&allow_all_mask) == -1) {
    perror("parent-sigfillset");
    return 1;
  }

  action.sa_handler = &handler;
  action.sa_mask = allow_all_mask;
  action.sa_flags =
      SA_RESTART;  // When a signal is delivered during certain primitives (such
                   // as open, read or write), returning from a handler resumes
                   // the library function.

  sigaction(SIGINT, NULL, &query_action);
  if (query_action.sa_handler != SIG_IGN) {
    if (sigaction(SIGINT, &action, NULL) == -1) {
      perror("sigaction");
      return 1;
    }
  }
  sigaction(SIGCHLD, NULL, &query_action);
  if (query_action.sa_handler != SIG_IGN) {
    if (sigaction(SIGCHLD, &action, NULL) == -1) {
      perror("sigaction");
      return 1;
    }
  }

  printf("[Manager] Spawning child\n");

  pid_t cpid = fork();
  if (cpid == -1) {
    perror("parent-fork");
    return 1;
  }

  if (cpid == 0) {
    execl("./child", "./child", (char *)NULL);
    perror("child-execl");
    return 1;
  }

  while (keep_running) {
    printf("[Parent] No stopped child\n");
    // TODO: Block to check.
    int collected = 0;
    if (stopped_child) {
      printf("[Parent] A child process has stopped\n");
      if ((collected = collect(cpid)) == -1) {
        perror("parent-collect");
        return 1;
      }
      if (collected != 0) {
        printf("[Parent] Sending SIGCONT signal to child ... ");
        fflush(stdout);
        kill(cpid, SIGCONT);
        printf("Done\n");
      }
      stopped_child = 0;
    }
    // TODO Release lock after check.

    sleep(1);
  }

  printf("[Parent] Exiting...\n");
  if (kill(cpid, SIGTERM) == -1) {
    perror("parent-kill");
    return 1;
  }
  collect(cpid);

  return 0;
}
