#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void* background_thread(void* arg) {
#define N_BACK 10
  int count = 0;
  for (int i = 0; i < N_BACK; ++i) {
    sleep(2);

  }

  return NULL
}
