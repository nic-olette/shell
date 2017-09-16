#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

void printDetails (struct stat *buff, char *name) {
    char type = '-';
    if (S_ISDIR(buff->st_mode)) 
      type = 'd';
    else if (S_ISLNK(buff->st_mode)) 
      type = 'l';

    char permissions[9];
    permissions[0] = (buff->st_mode & S_IRUSR) ? 'r' : '-';
    permissions[1] = (buff->st_mode & S_IWUSR) ? 'w' : '-';
    permissions[2] = (buff->st_mode & S_IXUSR) ? 'x' : '-';
    permissions[3] = (buff->st_mode & S_IRGRP) ? 'r' : '-';
    permissions[4] = (buff->st_mode & S_IWGRP) ? 'w' : '-';
    permissions[5] = (buff->st_mode & S_IXGRP) ? 'x' : '-';
    permissions[6] = (buff->st_mode & S_IROTH) ? 'r' : '-';
    permissions[7] = (buff->st_mode & S_IWOTH) ? 'w' : '-';
    permissions[8] = (buff->st_mode & S_IXOTH) ? 'x' : '-';

    int link = buff->st_nlink;

    char username[32];
    struct passwd *user = getpwuid(buff->st_uid);
    strncpy(username, user->pw_name, 32);
	
    char groupname[32];
    struct group *grp = getgrgid(buff->st_gid);
    strncpy(groupname, grp->gr_name, 32);
	
    int size = buff->st_size;

    char time[50];
    strncpy(time, ctime(&buff->st_atime), 50);
    time[strlen(time)-1] = 0;
    
    printf("%c%s%3d %s %s%8d %s %s\n", 
            type, permissions, link, username, groupname, size, time, name);
}

int main(int argc, char **argv) {
  char *name;
  int c, lflag = 0;
 
  while ((c = getopt(argc, argv, "Ll")) != -1) {
    switch (c) {
      case 'l':
        lflag = 1;
	break;
      case 'L':
        lflag = 1;
	break;
      case '?':
	fprintf(stderr, "incorrect usage"); 
        exit(EXIT_FAILURE);
    }
  }

  //ls -l file/dir
  if (lflag == 1 && argc == 3) { 
    name = argv[2];

    struct stat buff;
    if ((lstat(name, &buff)) < 0) //get file deets
      perror("myls:");

    if (S_ISDIR(buff.st_mode)) { //if dir, print sorted contents
      int i, j;
      struct dirent **list;
      i = scandir(name, &list, NULL, alphasort);
      for (j = 0; j < i; j++) {
        lstat(list[j]->d_name, &buff);
        printDetails(&buff, list[j]->d_name);
      }
    }
    else //not a directory, print single detail entry
      printDetails(&buff, name);
  }

  //ls -l 
  else if (lflag == 1 && argc == 2) { 
    name = ".";

    struct stat buff;
    if ((lstat(name, &buff)) < 0) //get file deets
      perror("myls:");

    if (S_ISDIR(buff.st_mode)) { //if dir, print sorted contents
      int i, j;
      struct dirent **list;
      i = scandir(name, &list, NULL, alphasort);
      for (j = 0; j < i; j++) {
        lstat(list[j]->d_name, &buff);
        printDetails(&buff, list[j]->d_name);
      }
    }
  }

  //ls file/dir
  else if (lflag == 0 && argc == 2) { 
    name = argv[1];

    struct stat buff;
    if ((lstat(name, &buff)) < 0) //get file deets
      perror("myls:");

    if (S_ISDIR(buff.st_mode)) { //if dir, print contents
      int i, j;
      struct dirent **list;
      i = scandir(name, &list, NULL, NULL);
      for (j = 0; j < i; j++) {
        if (strcmp(list[j]->d_name, ".") != 0 && strcmp(list[j]->d_name, "..") != 0)
          printf("%-15s", list[j]->d_name);
      }
    }
    else //not a directory, print name
      printf("%s\n", name);
  }

  //just ls
  else if (lflag == 0 && argc == 1) {
    name = ".";
    int i, j;
    struct dirent **list;
    i = scandir(name, &list, NULL, NULL);
    for (j = 0; j < i; j++) {
      if (strcmp(list[j]->d_name, ".") != 0 && strcmp(list[j]->d_name, "..") != 0)
        printf("%-s	", list[j]->d_name);
    }
    printf("\n");
  }
  else
    printf("shit broke");

  return 0;
}
