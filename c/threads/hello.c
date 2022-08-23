#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void* routine() {
  static int n = 5;
  printf("Hello from threads\n");
  sleep(3);
  printf("Ending thread\n");

  return &n;
}

int main(int argc, char const* argv[]) {
  pthread_t p1, p2;

  int n = -1;
  if (pthread_create(&p1, NULL, &routine, NULL) != 0) {
    return 1;
  }

  if (pthread_join(p1, &n) != 0) {
    return 1;
  }

  printf("N: %d\n", n);

  return 0;
}
