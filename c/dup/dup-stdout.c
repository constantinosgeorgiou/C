#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define PATH "dup-stdout-file.txt"

int main(int argc, char const *argv[]) {
  // Processes(2)
  // - Parent
  // - Child
  //
  // Child
  //  - Redirect STDOUT to file_fd
  //  - Write to file_fd using printf.
  // PArent waits for child to finish.

  pid_t cpid = fork();
  if (cpid == -1) {
    perror("fork");
    return 1;
  }

  if (cpid == 0) {
    //
    // Child
    //
    int flags = O_WRONLY | O_CREAT | O_TRUNC;
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    int file_fd = open(PATH, flags, mode);
    if (file_fd == -1) {
      perror("open");
      exit(EXIT_FAILURE);
    }

    // Redirect standard output to file_fd
    int stdout_copy = dup(STDOUT_FILENO);
    if (stdout_copy == -1 && errno != EBADF) {
      perror("dup-stdout");
      exit(EXIT_FAILURE);
    }
    if (dup2(file_fd, STDOUT_FILENO) == -1) {
      perror("dup2");
      exit(EXIT_FAILURE);
    }
    if (close(file_fd) == -1) {
      perror("close-file_fd");
      exit(EXIT_FAILURE);
    }

    printf("Now you can write to file using printf()\n");
    printf("wow amazing\n");
    printf("even numbers 123456789\n");
  }

  //
  // Parent
  //
  if (cpid != 0) {
    printf("Parent: waiting for child %d ... ", cpid);
    fflush(stdout);
    waitpid(cpid, 0, 0);
    printf("done\n");
  }

  return 0;
}
