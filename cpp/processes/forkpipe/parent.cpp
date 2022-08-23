// Program to understand how pipes work.
// Two processes. The child pipes strings ending with '\n' to parent.
// Parent reads string and outputs them. Child can pipe a single string,
// or multiple ones at once.
//
// Goal: Understand when a pipe gets cleared. Successfully manipulate the
//       pipe.

#include <sys/wait.h>
#include <unistd.h>

#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <queue>
#include <string>

using namespace std;

static volatile sig_atomic_t keep_running = true;

int cleanup(pid_t cpid) {
  printf("\nExiting ...\n");
  fflush(stdout);
  if (cpid > 0) {
    printf("Terminating child (%d) ... ", cpid);
    if (kill(cpid, SIGINT) == -1) {
      printf("error\n");
      perror("kill");
      return -1;
    }
    int status = 0;
    waitpid(cpid, &status, 0);
    if (status == -1 && errno != ECHILD) {
      printf("error\n");
      perror("waitpid");
      return -1;
    }
    printf("done\n");
  }
  // TODO: maybe close fds.
  printf("Exited \033[32m✓\033[0m\n");

  return 0;
}

void handle_signal(int signal) {
  if (signal == SIGINT) {
    keep_running = false;
  }
}

int main(int argc, char const* argv[]) {
  struct sigaction act;
  act.sa_handler = handle_signal;
  sigfillset(&act.sa_mask);  // Block all signals.
  sigaction(SIGINT, &act, NULL);

  int fd[2] = {0};
  if (pipe(fd) == -1) {
    perror("pipe");
    return 1;
  }

  pid_t cpid = fork();
  if (cpid == -1) {
    perror("fork");
    if (cleanup(cpid) == -1) {
      perror("cleanup");
      return 1;
    }
    return 1;
  }
  if (cpid == 0) {
    // Redirect output
    // ---------------
    if (close(fd[0]) == -1) {
      perror("child-close-fd[0]");
      if (cleanup(cpid) == -1) {
        perror("cleanup");
        return 1;
      }
      return 1;
    }
    if (dup2(fd[1], STDOUT_FILENO) == -1) {
      perror("child-dup2-fd[1]-stdout");
      if (cleanup(cpid) == -1) {
        perror("cleanup");
        return 1;
      }
      return 1;
    }
    if (close(fd[1]) == -1) {
      perror("child-close-fd[1]");
      if (cleanup(cpid) == -1) {
        perror("cleanup");
        return 1;
      }
      return 1;
    }
    execl("./child", "./child", nullptr);
    perror("execl-child");
    if (cleanup(cpid) == -1) {
      perror("cleanup");
      return 1;
    }
    return 1;
  }

  // Parent only read from pipe. Write-end is useless.
  if (close(fd[1]) == -1) {
    perror("parent-close-fd[1]");
    if (cleanup(cpid) == -1) {
      perror("cleanup");
      return 1;
    }
    return 1;
  }

  std::queue<std::string> qstrings;

  // Get maximum number of bytes that can be written atomically to pipe.
  size_t capacity = fpathconf(fd[0], _PC_PIPE_BUF);

  char buffer[capacity];
  memset(buffer, 0x00, capacity);  // Initialize buffer to 0;

  while (keep_running) {
    // Read strings
    // ------------

    printf("[Parent] Awaiting strings ... ");
    if (read(fd[0], buffer, capacity) == -1) {
      perror("parent-read-fd[0]");
      if (cleanup(cpid) == -1) {
        perror("cleanup");
        return 1;
      }
      return 1;
    }
    printf("received\n");

    // Parse strings
    // -------------

    printf("[Manager] Parsing strings ... ");
    int tokens = 0;
    char* context = nullptr;
    char* token = strtok_r(buffer, "\n", &context);
    while (token != nullptr) {
      tokens++;
      string file = token;
      qstrings.push(file);                        // Track files.
      token = strtok_r(nullptr, "\n", &context);  // Next token.
    }
    printf("parsed\n");

    // Output strings
    // --------------

    printf("[Manager] Received: \n");
    while (!qstrings.empty()) {
      string front = qstrings.front();
      printf("          - '%s'\n", front.c_str());
      qstrings.pop();
    }

    // Reset process
    // -------------

    tokens = 0;                      // Reset token counter.
    memset(buffer, 0x00, capacity);  // Set buffer to 0.
  }

  // Close read-end of pipe.
  if (close(fd[0]) == -1) {
    perror("parent-close-fd[0]");
    if (cleanup(cpid) == -1) {
      perror("cleanup");
      return 1;
    }
    return 1;
  }
  if (cleanup(cpid) == -1) {
    perror("cleanup");
    return 1;
  }

  return 0;
}
