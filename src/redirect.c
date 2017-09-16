#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "parser.h"
#include "redirect.h"

void redirection(char **args, struct Flags *fl) {
  int childPid, status, fd;
  childPid = fork();
  if (childPid == 0) {
    if (fl->redirINflag > 0) {
      fd = open(args[fl->redirINflag], O_RDONLY);
      dup2(fd,0);
      close(fd);
    }
    if (fl->redirOUTflag > 0) {
      fd = open(args[fl->redirOUTflag], O_WRONLY|O_CREAT|O_TRUNC, 0600);
      dup2(fd,1);
      close(fd);
    }
    if ((strncmp(args[0], "my", 2)) == 0)
      execv(args[0], args);
    else
      execvp(args[0], args);
    }
  else //parent waits
    waitpid(childPid, &status, 0);
}

void pipes(char **args) {
  int pid1, pid2, status;
  int pp[2];
  if (pipe(pp) < 0) exit(EXIT_FAILURE); //pipe fail

  char *a1[2], *a2[2];
  a1[0]=args[0];
  a1[1]=NULL;
  a2[0]=args[2];
  a2[1]=NULL;

  pid1=fork();
  if (pid1 == 0) { // execute first program
    dup2(pp[1], 1);
    close(pp[0]);
    if ((strncmp(args[0], "my", 2)) == 0)
      execv(a1[0], a1);
    else
      execvp(a1[0], a1);
  }

  pid2=fork();
  if (pid2 == 0) { // execute second program
    dup2(pp[0], 0);
    close(pp[1]);
    if ((strncmp(args[0], "my", 2)) == 0)
      execv(a2[0], a2);
    else
      execvp(a2[0], a2);
    }

  if (pid1 > 0 && pid2 > 0) {//parent waits
    close(pp[0]);
    close(pp[1]);
    waitpid(pid2, &status, 0);
  }
}
