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

    srand(time(NULL));
    int capacity = rand() % 15;
    int size = rand() % capacity + 1;  // 1..capacity
    int arr[capacity];
    memset(arr, 0, capacity);
    printf("[ Child] array -> (capacity: %d, size:%d)\n", capacity, size);
    printf("[ Child] generated ");
    for (int i = 0; i < size; i++) {
      arr[i] = rand() % 11;  // 0..10
      printf("%d ", arr[i]);
    }
    printf("\n");

    printf("[ Child] writing size of array into pipe\n");
    if (write(pipe_fd[1], &size, sizeof(int)) == -1) {
      return 1;
    }
    printf("[ Child] writing array into pipe\n");
    if (write(pipe_fd[1], arr, sizeof(int) * size) == -1) {
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

    int size = 0;
    if (read(pipe_fd[0], &size, sizeof(int)) == -1) {
      return -1;
    }
    int arr[size];
    if (read(pipe_fd[0], arr, sizeof(int) * size) == -1) {
      return -1;
    }
    printf("[Parent] array -> size: %d\n", size);
    

    int sum = 0;
    printf("[Parent] received ");
    for (int i = 0; i < size; i++) {
      printf("%d ", arr[i]);
      sum += arr[i];
    }
    printf("\n          sum: %d\n", sum);

    if (close(pipe_fd[1]) == -1) {
      return 1;
    }

    waitpid(cpid, 0, 0);
  }

  return 0;
}
