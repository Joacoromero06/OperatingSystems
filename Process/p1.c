#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int 
main(int argc, char* argv[])
{
  printf("hello (pid %d)\n", (int) getpid());
  int rc_fork = fork();
  if (rc_fork < 0){
    printf("fork failed\n");
    exit(1);
  }
  else if (rc_fork == 0){
    printf("child (pid %d)\n", getpid());
  } else{
    printf("(pid %d) is parent of (pid %d)\n", getpid(), rc_fork);
  }
  return 0;
}
