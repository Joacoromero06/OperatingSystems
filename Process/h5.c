#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <assert.h>

int main(int argc, char* argv[]) {
  int rc_fk = fork();
  assert(rc_fk >= 0);
  
  int rc_wt;
  if (rc_fk == 0){
    printf("(pid %d)\t child process waiting...\n", getpid());
    rc_wt = wait(NULL);
    printf("rc_wt=%d from child waiting\n", rc_wt);
  } else{
    printf("(pid %d)\t parent process waiting child process...\n", getpid());
    rc_wt = wait(NULL);
    printf("rc_wt=%d from parent waitng\n", rc_wt);
  }
  return 0;
}

