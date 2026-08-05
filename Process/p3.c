#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <assert.h>

#define ASSERT_FORK(rc) (assert(rc)>=0);

int 
main(int argc, char* argv[])
{
  printf("hello from (pid %d)\n", getpid());
  int rc = fork();
  assert(rc >= 0);
  if (rc == 0){
    printf("child process forked (pid %d)", getpid());
    char* args[3];
    args[0] = "wc";
    args[1] = "p3.c";
    args[2] = NULL;
    execvp(args[0], args);
    printf("no se muestra, exec finaliza cambia de programa al proceso\n");
  } else{
    int rc_wait = wait(NULL);
    printf("(pid %d) is parent of (pid child %d), and has waited with rc=%d\n",
            getpid(), rc, rc_wait);
  }
  return 0;
}
  
