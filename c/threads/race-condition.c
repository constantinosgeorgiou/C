#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int mails = 0;

void* routine() {
  for (int i = 0; i < 100000; i++) {
    mails++;
  }
}

int main(int argc, char const* argv[]) {
  pthread_t p1, p2;

  if (pthread_create(&p1, NULL, &routine, NULL) != 0) {
    return 1;
  }
  if (pthread_create(&p2, NULL, &routine, NULL) != 0) {
    return 1;
  }

  if (pthread_join(p1, NULL) != 0) {
    return 1;
  }
  if (pthread_join(p2, NULL) != 0) {
    return 1;
  }

  printf("Number of mails: %d\n", mails);

  return 0;
}
