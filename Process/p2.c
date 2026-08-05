#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int
main(int argc, char* argv[])
{
  printf("Hello (pid %d)\n", getpid());
  int rc_fork = fork();
  if (rc_fork < 0){
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc_fork == 0){
    printf("child (pid %d)\n", getpid());
  } else {
    int rc_wait = wait(NULL);
    printf("(pid %d) has waited (rc_wait %d), and is parent of (pid %d)\n", 
            getpid(), rc_wait, rc_fork);
  }
  return 0;
}
