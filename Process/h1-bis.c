#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <assert.h>

#define ASSERT_FORK(rc) assert(rc>=0)
#define CHILD_VALUE 1
#define PARENT_VALUE 1000
int 
main(int argc, char *argv[])
{
  int rc = fork();
  int x = 100; // shared variable??
  ASSERT_FORK(rc);
  if (rc == 0){
    printf("(pid %d) child process: x=%d\n", getpid(), x);
    for(int i=0; i <= 10; i++){ printf("waiting for parent to change x: %d\n", i);}
    x = CHILD_VALUE;
    printf("child process modify x: x=%d\n", x);
  } else{
    wait(NULL);
    printf("(pid %d) parent process: x=%d\n", getpid(), x);
    x = PARENT_VALUE;
    //wait(NULL);
    printf("parent process modify x: x=%d\n", x);
  }
  return 0;
}
