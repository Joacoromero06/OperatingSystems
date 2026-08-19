#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <assert.h>
#include <stdlib.h>


#define prog "/bin/ls"
#define dir "../Process"


extern char** environ;
int 
main(int argc, char* argv[])
{
  int opt;
  if (argc == 1){
    fprintf(stderr, "argc = 0\n");
    opt = 0;
  } else{
    opt = atoi(argv[1]);
  }
  int rc_fk = fork();
  assert(rc_fk >= 0);
  if (rc_fk == 0){
    char* argsv[3];
    argsv[0] = prog; argsv[1] = dir; argsv[2] = NULL;
    char* argse[1];
    argse[0] = NULL;
    switch(opt) {
      //case 0: exec(F); break;
      case 0: execve(prog, argsv, argse); break;
      case 1: execl(prog, "ls", dir, (char*) NULL); break;
      case 2: execle(prog, "ls", dir, (char*) NULL, environ); break;
      case 3: execlp("ls", "ls", dir, (char*) NULL); break;
      case 4: execv(prog, argsv); break;
      case 5: execvp("ls", argsv); break;
      //case 6: execvpe(F); break;
    }
  }
  else{ 
    wait(NULL);
    printf("finishing execution of /bin/ls\n");
  }
  return 0;
}
