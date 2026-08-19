#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/mman.h>
#include <assert.h>

int main(int argc, char* argv[])
{
  int* s_x = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
  assert(s_x != MAP_FAILED);
  *s_x = 0;

  int rc_fk = fork();
  assert(rc_fk >= 0);

  if (rc_fk == 0){
    printf("hello! \n");
    fflush(stdout);
    *s_x = 1;
  } else{
    while(*s_x == 0)
      ;
    printf("goodbye!\n");
  }
  return 0;
}
