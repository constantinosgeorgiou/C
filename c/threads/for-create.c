#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define THREADS 4

int mails = 0;
pthread_mutex_t mutex;

void* routine() {
  for (int i = 0; i < 1000000; i++) {
    pthread_mutex_lock(&mutex);
    mails++;
    pthread_mutex_unlock(&mutex);
  }

  return NULL;
}

int main(int argc, char const* argv[]) {
  pthread_t threads[THREADS];

  pthread_mutex_init(&mutex, NULL);

  for (int i = 0; i < THREADS; i++) {
    if (pthread_create(&threads[i], NULL, &routine, NULL) != 0) {
      return 1;
    }
    printf("Thread %d started\n", i);
  }

  for (int i = 0; i < THREADS; i++) {
    if (pthread_join(threads[i], NULL) != 0) {
      return 1;
    }
    printf("Thread %d done\n", i);
  }

  pthread_mutex_destroy(&mutex);

  printf("Number of mails: %d\n", mails);

  return 0;
}
