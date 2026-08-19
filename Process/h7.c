#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
  int rc_fk = fork();
  if (rc_fk == 0){
    close(STDOUT_FILENO);
    printf("printing after closing file descriptor for standard output\n");
  } else{
    wait(NULL);
    printf("printing after waiting for child process closing fd stdout\n");
  }
  return 0;
}
