#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <stddef.h>
#include <stdio.h>

#define dbg(string) (puts(string))

/// p: pointer to buffer heap-stack allocated
void carga(char* p);

/// destination pointer of buffer is unitializated, creating UB, 
/// must be segmentation fault by man page of strcpy POSIX
void create_segfault();

int 
main()
{
  char* p = (char*) reallocarray(NULL, 32, sizeof(char));
  if (p == NULL)
    err(EXIT_FAILURE, "reallocarray error\n");

  carga(p); dbg(p);
  create_segfault(); dbg(p);
}

void
carga(char* p)
{
  strlcpy(p, "Hola puedo poner 32.\n", 32);
  strlcat(p, "Me salgo de 32.\n", 32);
}

void 
create_segfault()
{
  char* src = "p (char*) --> [this buf]";
  char* dest;
  strcpy(dest, src);
}
