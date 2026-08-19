#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <assert.h>
#include <fcntl.h>
#include <string.h>

#define FILEPATH "./test-file.txt"
#define FILEPATH_SOURCE "./file.txt"
#define BUF_SIZE 1024
#define ASSERT_FORK(rc) (assert(rc>=0))
#define ASSERT_OPEN(fd) assert(fd>=0)
#define ASSERT_READ(rc) assert(rc>=0)

void show_buf(char* buf, size_t n){
  int i = 0;
  while(buf[i] != '\0');
}
int 
main(int argc, char* argv[])
{
  int fd = open(FILEPATH, O_RDWR); // we suppose FILE exists already
  int fd_source = open(FILEPATH_SOURCE, O_RDONLY);
  ASSERT_OPEN(fd);
  char buf[BUF_SIZE];

  int rc = fork();
  ASSERT_FORK(rc);
  if (rc==0){ // child reads from test.txt to buf and then writes to the final of test-file.txt
    printf("(pid %d)\tchild process reading %s to buffer...\n\n", getpid(), FILEPATH_SOURCE);
    int rc_read = read(fd_source, buf, BUF_SIZE-1);
    buf[rc_read] = '\0';
    printf("%s\n", buf);
    printf("(pid %d)\tchild process writting buffer to %s\n\n", getpid(), FILEPATH);
    int rc_wr = write(fd, buf, strlen(buf));
    printf("(pid %d)\tchild process leaving...\n\n\n", getpid());
  } else{ // parent reads from test-file after child and then writes '$$$' at the beggining of test-file.txt
    wait(NULL);
    printf("(pid %d)\tparent process reading from %s to buffer...\n\n", getpid(), FILEPATH);
    int rc_read = read(fd, buf, BUF_SIZE-1);
    printf("rc_read=%d\n", rc_read);
    if (rc_read == 0){
      buf[BUF_SIZE-1] = '\0';
    } else{
      buf[rc_read] = '\0';
    }
    ASSERT_READ(rc_read);
    printf("%s\n", buf);
    int rc_wr = write(fd, "$$$", 4);

  }
  return 0;
}
    


