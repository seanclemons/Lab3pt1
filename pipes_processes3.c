
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char ** argv)
{
  if (argc != 2)
  {
    fprintf(stderr, "Usage: %s <search_term>\n", argv[0]);
    exit(1);
  }

  int pipefd1[2];
  int pipefd2[2];
  int pid1, pid2;

  if (pipe(pipefd1) == -1) {
    perror("pipe");
    exit(1);
  }

  if ((pid1 = fork()) == -1) {
    perror("fork");
    exit(1);
  }
  if (pid1 == 0){

    if (pipe(pipefd2) == -1) {
      perror("pipe");
      exit(1);
    }

    if ((pid2 = fork()) == -1){
      perror("fork");
      exit(1);
    }  

    if (pid2 == 0) {
      dup2(pipefd2[0], 0);
      close(pipefd2[1]);
      close(pipefd2[0]);

      execlp("sort", "sort", NULL);
      perror("execlp failed for sort");
    }
    else { 
      dup2(pipefd1[0], 0);
      dup2(pipefd2[1], 1);
      
      close(pipefd1[1]);
      close(pipefd1[0]);
      close(pipefd2[0]);
      close(pipefd2[1]);

      execlp("grep","grep", argv[1], NULL);
      perror("execlp failed for grep");
    }
  }

  else {
    dup2(pipefd1[1], 1);
    close(pipefd1[0]);
    close(pipefd1[1]);

    execlp("cat", "cat", "scores", NULL);
    perror("execlp failed for cat");
  }

  return 0;
}
