#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define THREADS 4

pthread_mutex_t mutex;

void* routine(void* args) {
  // pthread_mutex_lock(&mutex);
  if (pthread_mutex_trylock(&mutex) == 0) {
    printf("Got lock\n");
    sleep(1);
    pthread_mutex_unlock(&mutex);
  } else {
    printf("Didn't get lock\n");
  }
}

int main(int argc, char const* argv[]) {
  pthread_t threads[THREADS];

  pthread_mutex_init(&mutex, NULL);

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

  pthread_mutex_destroy(&mutex);

  return 0;
}
