#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "parser.h"
#include "redirect.h"

void prompt();
void mycd(char *path);

int main (int argc, char **argv)
{
  prompt();

  //get user command
  char line[1026];
  fgets(line, 1026, stdin);

  //process commands until "exit"
  while((strncmp(line, "exit", 4)) != 0)
  {
    char *args[512];
    struct Flags fl = {0,0,0,0};
    int childPid, status;

    //parse command line
    parse(line, args, &fl);

    //execute commands
    if (fl.redirINflag > 0 || fl.redirOUTflag > 0) { //with redirection
      redirection(args, &fl);
    }
    else if (fl.pipeflag > 0) { //with pipe
      pipes(args);
    }
    else if (fl.cdflag == 0) { //no redirection && didn't change dir
      childPid = fork();
      if (childPid == 0)
        if ((strncmp(args[0], "my", 2)) == 0)
          execv(args[0], args);
        else //use command path commands
          execvp(args[0], args);
      else //parent waits
        waitpid(childPid, &status, 0);
    }

    //get new command
    prompt();
    fgets(line, 1026, stdin);
  }

  return 0;
}


void prompt() {
  struct passwd *user = getpwuid(getuid());
  char *cwd = getcwd(NULL, 0);
  strcpy(cwd, &cwd[strlen(user->pw_dir)]);
  printf("%s:~%s$ ", user->pw_name, cwd); //user:cwd$
}


void mycd(char *path) {
   char cwd[512];
   getcwd(cwd,sizeof(cwd));

   strcat(cwd,"/");
   strcat(cwd,path);
   chdir(cwd);
}
