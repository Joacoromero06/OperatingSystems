#ifndef __common_thread_h__
#define __common_thread_h__

#include <pthread.h>
#include <assert.h>
#include <sched.h>

#ifdef __linux__
#include <semaphore.h>
#endif

#define Pthread_create(thread, attr, start_routine, arg) \
  assert(pthread_create(thread, attr, start_routine, arg) == 0);
#define Pthread_join(thread, value_ptr) \
  assert(pthread_join(thread, value_ptr) == 0);

#endif // __common_thread_h ifn
