#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define THREADS 2

int primes[10] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

void* routine(void* arg) {
  int index = *(int*)arg;

  int sum = 0;
  for (int i = 0; i < 5; i++) {
    sum += primes[index + i];
  }

  *(int*)arg = sum;
  return arg;
}

int main(int argc, char const* argv[]) {
  pthread_t threads[THREADS];

  for (int i = 0; i < THREADS; i++) {
    int* index = malloc(sizeof(*index));
    *index = i * 5;
    if (pthread_create(&threads[i], NULL, &routine, index) != 0) {
      perror("pthread_create");
      return 1;
    }
  }

  int sum = 0;
  for (int i = 0; i < THREADS; i++) {
    int* result;
    if (pthread_join(threads[i], (void**)&result) != 0) {
      perror("pthread_join");
      return 1;
    }
    sum += *result;
    free(result);
  }

  printf("Sum: %d\n", sum);

  return 0;
}
