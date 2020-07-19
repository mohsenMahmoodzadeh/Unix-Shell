##########Variables
CC=g++
CFLAGS=-c -Wall
RMFLAGS=-rf

##########Rules

Shell: Shell.o
	$(CC) Shell.o -o Shell 

Shell.o: Shell.c
	$(CC) $(CFLAGS) Shell.c


