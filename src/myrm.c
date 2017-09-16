#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

char *pathCat(const char *original, const char *addition) {
  char *newPath = ((char *) malloc(2 + (strlen(original)) + (strlen(addition))));
  const char *slash = "/";
  strcpy(newPath, original);
  strcat(newPath, slash);
  strcat(newPath, addition);
  return newPath;
}

void deleteFile(const char *target) {
  unlink(target);
}

void deleteDir(const char *target) {
  //open target directory
  DIR *d;
  d=opendir(target);
  struct dirent *de;
  struct stat buff;

  //scan target dir until end
  de=readdir(d);
  while (de) {
    //set new path
    char *newTarget = pathCat(target, de->d_name);

    //get file characteristics
    if (lstat(newTarget, &buff) < 0)
      perror("lstat error");

    //if directory, that's not . or .., recurse
    if (S_ISDIR(buff.st_mode) && strcmp(de->d_name, ".") != 0
         && strcmp(de->d_name, "..") != 0)
      deleteDir(newTarget);

    //if file, unlink it
    if (S_ISREG(buff.st_mode)) {
     unlink(newTarget);
    }
    de=readdir(d);
    free(newTarget);
  }
  closedir(d);

  //finally remove target directory
  rmdir(target);
}


int main(int argc, char **argv)
{
int rflag = 0;
  int c;
  struct stat buff;
  opterr = 0;

  while ((c = getopt(argc, argv, "Rr")) != -1) {
    switch (c) {
      case 'R':
        rflag = 1;
        break;
      case 'r':
        rflag = 1;
        break;
      case '?':
        fprintf (stderr, "USAGE: myrm file OR mycp -R directory\n");
        exit(-1);
    }
  }

  //check for correct number of arguments
  if ((argc-optind) != 1) {
    fprintf(stderr, "USAGE: myrm file OR myrm -R directory\n"); 
    exit(EXIT_FAILURE);
  }

  //argument non-existant or incorrect format
  if ((lstat(argv[optind], &buff)) < 0) {
    fprintf(stderr, "Target file or directory does not exist.\n"); 
    exit(EXIT_FAILURE);
  }
  else if (rflag == 1 && !S_ISDIR(buff.st_mode)) {
    fprintf(stderr, "Target must be a directory with -R option.\n"); 
    exit(EXIT_FAILURE);
  }

  //argument is a file && rflag not set
  else if (rflag == 0 && S_ISREG(buff.st_mode))
    deleteFile(argv[optind]);

  //argument is directory && rflag set
  else if (rflag == 1 && S_ISDIR(buff.st_mode))
    deleteDir(argv[optind]);

  else {
    fprintf(stderr, "USAGE: myrm file OR myrm -R directory\n"); 
    exit(EXIT_FAILURE);
  }

  return 0;
}
