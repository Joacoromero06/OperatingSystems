#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>

#define ASSERT_FORK(rc) assert(rc>=0)
int 
main(int argc, char* argv[])
{
  int rc = fork();
  ASSERT_FORK(rc);
  if (rc == 0){
    close(STDOUT_FILENO);
    open("./p4.output", O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
    char* args[3];
    args[0] = strdup("wc");
    args[1] = strdup("p4.c");
    args[2] = NULL;
    execvp(args[0], args);
  } else{
    int rc_wait = wait(NULL);
  }
  return 0;
}

