#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "parser.h"

void parse(char *line, char **args, struct Flags *fl) {
  int i = 0;
  args[i] = strtok(line, " \n");

  while (args[i] != NULL) {
    if ((strncmp(args[i], "mycd", 4)) == 0) {
      i++;
      args[i] = strtok(NULL, "\n");
      fl->cdflag = 1;
      return;
    }
    else {
      char *temp = args[i];
      if(temp[0] == '>') {
        fl->redirOUTflag = i;
        strcpy(args[i], &temp[1]);
      }
      else if (temp[0] == '<') {
        fl->redirINflag = i;
        strcpy(args[i], &temp[1]);
      }
      else if (temp[0] == '|') {
        fl->pipeflag = 1;
      }
      i++;
      args[i] = strtok(NULL, " \n");
    }
  }
}
