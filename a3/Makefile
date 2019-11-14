CC = gcc
CFLAGS = -g -Wall
LDFLAGS= -pthread 
TARGETS= testA testB

all : $(TARGETS)

testA : rthread.o testA.o
	$(CC) $(CFLAGS) -o testA rthread.o testA.o $(LDFLAGS)

testB : rthread.o testB.o
	$(CC) $(CFLAGS) -o testB rthread.o testB.o $(LDFLAGS)

testA.o : rthread.o testA.c A3a.c
	$(CC) $(CFLAGS) -c testA.c

testB.o : rthread.o testB.c A3b.c
	$(CC) $(CFLAGS) -c testB.c

rthread.o : rthread.c rthread.h
	$(CC) $(CFLAGS) -c rthread.c

clean :
	rm -rf *.o testA testB
