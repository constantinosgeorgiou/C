#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char const *argv[]) {
  int pipe_fd[2] = {0};
  if (pipe(pipe_fd) == -1) {
    return 1;
  }

  pid_t cpid = fork();
  if (cpid == -1) {
    return 1;
  }

  if (cpid == 0) {
    // Child
    if (close(pipe_fd[0]) == -1) {
      return 1;
    }

    size_t buffer = 200;
    char str[buffer];
    printf("[ Child] input string: ");
    fgets(str, buffer, stdin);
    size_t length = strlen(str);
    str[length - 1] = '\0';  // Remove '\n' from the end of str.

    printf("[ Child] writing length of string into pipe\n");
    if (write(pipe_fd[1], &length, sizeof(size_t)) == -1) {
      return 1;
    }
    printf("[ Child] writing string into pipe\n");
    if (write(pipe_fd[1], str, length + 1) == -1) {
      return 1;
    }

    if (close(pipe_fd[1]) == -1) {
      return 1;
    }
  }

  if (cpid != 0) {
    // Parent
    if (close(pipe_fd[1]) == -1) {
      return 1;
    }

    size_t length = 0;
    if (read(pipe_fd[0], &length, sizeof(size_t)) == -1) {
      return -1;
    }

    char str[length];
    if (read(pipe_fd[0], str, length) == -1) {
      return -1;
    }
    printf("[Parent] string -> length: %ld\n", length);

    printf("[Parent] received '%s'\n", str);

    if (close(pipe_fd[1]) == -1) {
      return 1;
    }

    waitpid(cpid, 0, 0);
  }

  return 0;
}
