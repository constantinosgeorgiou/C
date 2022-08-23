#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define THREADS 6

int fuel = 0;
pthread_mutex_t fuel_mutex;
pthread_cond_t fuel_cond;

void* fuel_filling(void* arg) {
  for (int i = 0; i < 5; i++) {
    pthread_mutex_lock(&fuel_mutex);
    fuel += 30;
    printf("Filling fuel.... %d\n", fuel);
    pthread_mutex_unlock(&fuel_mutex);
    pthread_cond_broadcast(&fuel_cond);
    sleep(1);
  }
}

void* car(void* arg) {
  pthread_mutex_lock(&fuel_mutex);
  while (fuel < 40) {
    printf("No fuel, Waiting ... \n");
    pthread_cond_wait(&fuel_cond, &fuel_mutex);
  }

  fuel -= 40;
  printf("Got fuel. No left: %d\n", fuel);
  pthread_mutex_unlock(&fuel_mutex);
}

int main(int argc, char const* argv[]) {
  pthread_t threads[THREADS];

  pthread_mutex_init(&fuel_mutex, NULL);
  pthread_cond_init(&fuel_cond, NULL);

  for (int i = 0; i < THREADS; i++) {
    if (i == 4 || i == 5) {
      if (pthread_create(&threads[i], NULL, &fuel_filling, NULL) != 0) {
        perror("pthread_create-fuel_filling");
        return 1;
      }
    } else {
      if (pthread_create(&threads[i], NULL, &car, NULL) != 0) {
        perror("pthread_create-car");
        return 1;
      }
    }
  }

  for (int i = 0; i < THREADS; i++) {
    if (pthread_join(threads[i], NULL) != 0) {
      perror("pthread_join");
      return 1;
    }
  }

  pthread_mutex_destroy(&fuel_mutex);
  pthread_cond_destroy(&fuel_cond);
  return 0;
}
