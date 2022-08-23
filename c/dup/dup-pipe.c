#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define READ 0
#define WRITE 1

#define PATH "dup-pipe-file.txt"
#define N 10  // Numbers to output

int main(int argc, char const* argv[]) {
  // Processes(2)
  // - Parent
  // - Child
  //
  // Parent
  //  - Creates pipe.
  //  - Constantly reads numbers from pipe.
  //  - Outputs numbers to file.
  //  - Terminates after 10 seconds.
  //
  // Child
  //  - Redirect STDOUT to pipe_fd[WRITE].
  //  - Writes 10 numbers to pipe_fd[WRITE] using printf.

  int pipe_fd[2] = {0};
  if (pipe(pipe_fd) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  pid_t cpid = fork();
  if (cpid == -1) {
    perror("fork");
    return 1;
  }
  if (cpid == 0) {
    //
    // Child
    //

    if (close(pipe_fd[READ]) == -1) {
      perror("close-pipe_fd[READ]");
      exit(EXIT_FAILURE);
    }

    // Redirect standard output to file_fd
    int stdout_copy = dup(STDOUT_FILENO);
    if (stdout_copy == -1 && errno != EBADF) {
      perror("dup-stdout");
      exit(EXIT_FAILURE);
    }
    if (dup2(pipe_fd[WRITE], STDOUT_FILENO) == -1) {
      perror("dup2");
      exit(EXIT_FAILURE);
    }

    // Output n
    int n = N;
    printf("%d ", n);
    // Output numbers
    for (int i = 0; i < n; i++) {
      printf("%d", i);
      if (i < n - 1) {
        printf(" ");
      }
    }
    printf("\n");

    if (close(pipe_fd[WRITE]) == -1) {
      perror("close-pipe_fd[WRITE]");
      exit(EXIT_FAILURE);
    }
  }

  //
  // Parent
  //
  if (cpid != 0) {
    if (close(pipe_fd[WRITE]) == -1) {
      perror("close-pipe_fd[WRITE]");
      exit(EXIT_FAILURE);
    }

    // Read n.
    char buffer[256];
    if (read(pipe_fd[READ], buffer, sizeof(buffer)) == -1) {
      perror("read-pipe_fd[READ]");
      exit(EXIT_FAILURE);
    }

    printf("Parent: received '%s'\n", buffer);

    int flags = O_WRONLY | O_CREAT | O_TRUNC;
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    int file_fd = open(PATH, flags, mode);
    if (file_fd == -1) {
      perror("open");
      exit(EXIT_FAILURE);
    }

    if (write(file_fd, "Total numbers: ", 16)) {
      perror("write");
      exit(EXIT_FAILURE);
    }
    char* token = NULL;
    char* rest = buffer;
    token = strtok_r(rest, " ", &rest);
    if (write(file_fd, token, sizeof(token))) {
      perror("write");
      exit(EXIT_FAILURE);
    }
    if (write(file_fd, "\n", 1)) {
      perror("write");
      exit(EXIT_FAILURE);
    }
    // Write numbers
    while ((token = strtok_r(rest, " ", &rest))) {
      if (write(file_fd, token, sizeof(token))) {
        perror("write");
        exit(EXIT_FAILURE);
      }
      if (write(file_fd, " ", 1)) {
        perror("write");
        exit(EXIT_FAILURE);
      }
    }
    if (write(file_fd, "\n", 1)) {
      perror("write");
      exit(EXIT_FAILURE);
    }

    if (close(file_fd) == -1) {
      perror("close");
      exit(EXIT_FAILURE);
    }

    printf("Parent: waiting for child %d ... ", cpid);
    fflush(stdout);
    waitpid(cpid, 0, 0);
    printf("done\n");
  }

  return 0;
}
