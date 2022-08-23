#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define THREADS 4

int primes[10] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

void* routine(void* arg) {
  int index = *(int*)arg;
  printf("%d ", primes[index]);
  free(arg);
}

int main(int argc, char const* argv[]) {
  pthread_t threads[THREADS];

  for (int i = 0; i < THREADS; i++) {
    int* index = malloc(sizeof(*index));
    *index = i;
    if (pthread_create(&threads[i], NULL, &routine, index) != 0) {
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
