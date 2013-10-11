
# Ryan Gittins and Phillip Sime
#
# CS441: Project 1 Part 2
# 2013-10-10
#
#####################################################################
#
# Type "make" or "make myshell" to compile your code
# 
# Type "make clean" to remove the executable (and any object files)
#
#####################################################################

CC=gcc
CFLAGS=-Wall -g

all: myshell

myshell: myshell.c myshell.h support.o
	$(CC) -o myshell $(CFLAGS) myshell.c support.o

support.o: support.h support.c
	$(CC) -c -o support.o $(CFLAGS) support.c

clean:
	$(RM) myshell *.o
	$(RM) -rf *.dSYM
