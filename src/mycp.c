/*
mycp file1 file2
Copies the contents of file1 into file2. If file2 does not exist, it is created;
otherwise, file2 is silently overwritten with the contents of file1.

mycp -R dir1 dir2
Copy the contents of the directory dir1. If directory dir2 does not exist,
it is created. Otherwise, it creates a directory named dir1 within directory
dir2.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <string.h>

void copyFile(const char *source, const char *target) {
  char block[4096];
  int src, trgt;
  int bytesRead;

  src = open(source, O_RDONLY);
  trgt = open(target, O_WRONLY | O_CREAT | O_TRUNC,
               S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);

  while((bytesRead = read(src, block, sizeof(block))) > 0)
    write(trgt, block, bytesRead);
}

char *pathCat(const char *original, const char *addition) {
  char *newPath = ((char *) malloc(2 + (strlen(original)) + (strlen(addition))));
  const char *slash = "/";
  strcpy(newPath, original);
  strcat(newPath, slash);
  strcat(newPath, addition);
  return newPath;
}

void copyDir(const char *source, const char *target) {
  //if target doesn't exist, mkdir target
  struct stat buff;
  if ((lstat(target, &buff)) < 0)
    mkdir(target, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  //else if directory duplicate source in target
  else if (S_ISDIR(buff.st_mode)) {
    target = pathCat(target, source);
    mkdir(target, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  }
  //else something bad happened
  else {
    fprintf(stderr, "Destination must be directory or created with -R flag set.\n");
    exit(EXIT_FAILURE);
  }

  //open source directory
  DIR *d;
  d=opendir(source);
  struct dirent *de;

  //scan source dir until end
  de=readdir(d);
  while (de) {
    //set new paths
    char *newSource = pathCat(source,de->d_name);
    char *newTarget = pathCat(target, de->d_name);

    //get file characteristics
    if (lstat(newSource, &buff) < 0)
      perror("mycp cannot open file");

    //if directory, that's not . or .., recurse
    if (S_ISDIR(buff.st_mode) && strcmp(de->d_name, ".") != 0 
         && strcmp(de->d_name, "..") != 0)
      copyDir(newSource, newTarget);

    //if file, copy it
    if (S_ISREG(buff.st_mode))
      copyFile(newSource, newTarget);

    de=readdir(d);
    free(newSource);
    free(newTarget);
  }
  closedir(d);
}

int main(int argc, char **argv) {
  int rflag = 0;
  int c;
  struct stat buff;
  opterr = 0;
  char *arg1, *arg2;

  while ((c = getopt(argc, argv, "Rr")) != -1) {
    switch (c) {
      case 'R':
        rflag = 1;
        break;
      case 'r':
        rflag = 1;
        break;
      case '?':
        fprintf (stderr, "USAGE: mycp file1 file2 OR mycp -R dir1 dir2\n");
        exit(EXIT_FAILURE);
    }
  }

  //check to make sure correct number of arguments
  if ((argc-optind) != 2) {
    fprintf(stderr, "USAGE: mycp file1 file2 OR mycp -R dir1 dir2\n");
    exit(EXIT_FAILURE);
  }

  //get arguments from argv
  arg1 = argv[optind];
  arg2 = argv[optind+1];

  //arg1 non-existant or incorrect format
  if ((lstat(arg1, &buff)) < 0) {
    fprintf(stderr, "Source file or directory does not exist.\n");
    exit(EXIT_FAILURE);
  }
  //file && -r
  else if (rflag == 1 && !S_ISDIR(buff.st_mode)) {
    fprintf(stderr, "Source must be a directory with -R option.\n");
    exit(EXIT_FAILURE);
  }

  //file only
  else if (rflag == 0 && S_ISREG(buff.st_mode))
    copyFile(arg1, arg2);

  //directory && -r
  else if (rflag == 1 && S_ISDIR(buff.st_mode))
    copyDir(arg1, arg2);

  else {
    fprintf(stderr, "USAGE: mycp file1 file2 OR mycp -R dir1 dir2\n");
    exit(EXIT_FAILURE);
  }

  return 0;
}
