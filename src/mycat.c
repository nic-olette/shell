#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

void catout(int infd, int outfd)
{
   char x[2]; // holder for read items
   int cread; 
   while((cread = read(infd, x, 1)) != 0) {
     write(outfd, x, 1);
   }
}

int main(int argc, char **argv)
{
  struct stat buff;
  int fdin, fdout;

  if (argc == 1) {
    fstat(0, &buff);  
    if(S_ISDIR(buff.st_mode)) {
      fprintf(stderr, "USAGE: mycat [file]\n");
      exit(EXIT_FAILURE);
    }
    fdin = 0;
    fdout = 1;
    catout(fdin, fdout); 
  }
  else if (argc == 2 || argc == 3) {
    char *filename = argv[1];
    stat(filename, &buff);

    if (S_ISDIR(buff.st_mode)) {
      perror("mycat: ");
      exit(EXIT_FAILURE);
    }

    close(0); // free up fd0
    fdin = open(filename, O_RDONLY);
    if(fdin == -1) {
	perror("mycat: ");
        exit(EXIT_FAILURE);
    }

    fdout = STDOUT_FILENO;
    catout(fdin, fdout);
  }

  exit(EXIT_SUCCESS);
}
