#include <stdio.h>
#include <err.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define MALLOC_ARRAY(n, type) ( (type*) my_malloc_array(n , sizeof(type)) )
#define MALLOC(type) (MALLOC_ARRAY(1, type))

static inline void* my_malloc_array(size_t n, size_t size);
static inline void seg_fault1();
static inline void ex1(char* p);
void ex2(char* d, char* s);

int main() 
{
  char* p;

  p = MALLOC_ARRAY(32, char);
  if (p == NULL)
    err(EXIT_FAILURE, "realloc array error");
  ex1(p);
  seg_fault1();
  puts(p);
  ex2(MALLOC_ARRAY(32, char), p);
  ex2(NULL, p);
}

static inline void* 
my_malloc_array(size_t n, size_t size)
{
  return reallocarray(NULL, n, size);
}

static inline void 
seg_fault1()
{

  char* src = "hello";
  char* dest; 
  strcpy(dest, src);
  puts(dest);
}

static inline void 
ex1(char* p)
{
   strlcpy(p, "hola puedo poner 32.\n\0", 32);
   strlcat(p, "Me salgo de 32.\n", 32);
   puts(p);
}

void ex2(char* dst, char* src)
{
  memcpy(dst, src, strlen(src)+1 );
  puts(dst);  
}
