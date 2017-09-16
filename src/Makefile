DFLAGS = -g -Wall -o #dev flags
PFLAGS = -o	     #production flags

project: my*.c parser.c parser.h redirect.c redirect.h 
	gcc $(PFLAGS) mycat mycat.c
	gcc $(PFLAGS) mycp mycp.c
	gcc $(PFLAGS) myls myls.c
	gcc $(PFLAGS) mypwd mypwd.c
	gcc $(PFLAGS) myrm myrm.c
	gcc $(PFLAGS) mysh.sh mysh.c parser.c redirect.c


shell: mysh.c parser.c parser.h redirect.c redirect.h
	gcc $(DFLAGS) mysh.sh mysh.c parser.c redirect.c

mycat: mycat.c
	gcc $(DFLAGS) mycat mycat.c

mycp: mycp.c
	gcc $(DFLAGS) mycp mycp.c

myls: myls.c
	gcc $(DFLAGS) myls myls.c

mypwd: mypwd.c
	gcc $(DFLAGS) mypwd mypwd.c

myrm: myrm.c
	gcc $(DFLAGS) myrm myrm.c

clean:
	rm -f mycat mycp myls mypwd myrm mysh.sh

## make sure that *only* the files for the project (all *.c and Makefile) are
# in the same directory.  then run 'make zip' to generate project tarball. 

zip:
	tar -cvzf unixShell.tar.gz ./*
