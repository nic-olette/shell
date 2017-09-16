#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char **argv) {
  char *pwd;
  pwd = getcwd(NULL, 0);

  if(pwd != NULL)
    printf("%s\n", pwd);

  return 0;
}
