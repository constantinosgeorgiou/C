#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>

#define THREADS 2

void* routine(void* args) {
  pthread_t th = pthread_self();

  printf("Thread ID (pthread_self): %ld\n", th);
  printf("Thread ID (syscall): %d\n", (pid_t)syscall(SYS_gettid));
}

int main(int argc, char const* argv[]) {
  pthread_t threads[THREADS];

  for (int i = 0; i < THREADS; i++) {
    if (pthread_create(&threads[i], NULL, &routine, NULL) != 0) {
      perror("pthread_create");
      return 1;
    }
  }

  for (int i = 0; i < THREADS; i++) {
    if (pthread_join(threads[i], NULL) != 0) {
      perror("pthread_join");
      return 1;
    }
  }

  return 0;
}
