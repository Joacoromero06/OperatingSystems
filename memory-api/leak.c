//#ifndef ARENA_H
#include <stddef.h>
#include <stdint.h>

#ifndef ARENA_STDIO
  #include <stdio.h>
  #include <stdarg.h>
  #define ARENA_STDIO
#endif

#ifndef ARENA_ASSERT
  #include <assert.h>
  #define ARENA_ASSERT(a) (assert(a))
#endif

#define CAPACITY (1024)
typedef struct Region Region;
struct Region{ // heap region with (8+8+8)bytes of meta data at beggining
  Region* next;
  size_t capacity;
  size_t count;
  uintptr_t data[];
};

typedef struct{
  Region* begin;
  Region* end;
} Arena;

typedef struct {
  Region* region;
  size_t count;
}ArenaMark;

Region* new_region(size_t n);
void free_region(Region* r);



#define ARENA_BACKEND 
#define ARENA_BACKEND_LINUX_MMAP
#define ARENA_BACKEND_LIBC_MALLOC

//#if ARENA_BACKEND == ARENA_BACKEND_LIBC_MALLOC
  #include <stdlib.h>
  #define alloc(type) ((type*) malloc(sizeof(type)))
  #define alloc_bytes(bsize) ((void*) malloc(bsize))

  #define ASSERT_REGION assert 
  Region* new_region(size_t n){
    size_t bsize = sizeof(Region) + sizeof(uintptr_t) * n;
    Region* r = (Region*) alloc_bytes(bsize);
    ASSERT_REGION(r)
    r->next = NULL;
    r->capacity = n;
    r->count = 0;
    return r;
  }

  void free_region(Region* r){
    free(r);
  }


//#elif ARENA_BACKEND == ARENA_BACKEND_LINUX_MMAP
  #include <unistd.h>
  #include <sys/mman.h>
  
  Region* new_region(size_t capacity){
    size_t bsize = sizeof(Region) + sizeof(uintptr_t) * capacity;
    Region* r = mmap(NULL, bsize, PROT_READ | PROT_WRITE,  MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    ASSERT_REGION(r != MAP_FAILED);
    r->next = NULL;
    r->capacity = n;
    r->count = 0;
  }
  void free_region(Region* r){
    size_t bsize = sizeof(Region) + sizeof(uintptr_t) * r->capacity;
    int ret = munmap(r, bsize);
    ASSERT_REGION(ret == 0);
  }

#define TWO_PAGES 4096
void* arena_alloc(Arena* a, size_t bsize)
{
#define p_WSIZE_ENTER(a, wsize) (a->end->count + wsize <= a->end->capacity)
  size_t wsize = (bsize + sizeof(uintptr_t) - 1) / sizeof(uintptr_t);

  if (a->end == NULL)
  {
    assert(a->begin == NULL);
    if (wsize < TWO_PAGES)
    {
      wsize = TWO_PAGES;
    }
    a->end = new_region(wsize);
    a->begin = a->end;
  }

  while (!p_WSIZE_ENTER(a, wsize) && a->end->next != NULL)
  {

  }
}
//#else 
//#error "Arena Backend no conocido"
//#endif
