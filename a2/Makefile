CC = gcc
CFLAGS = -g -Wall
LDFLAGS= -pthread 
TARGETS= sema lock two cp birds gpus

all : $(TARGETS)

gpus : rthread.o gpus.o
	$(CC) $(CFLAGS) -o gpus rthread.o gpus.o $(LDFLAGS)

birds : rthread.o birds.o
	$(CC) $(CFLAGS) -o birds rthread.o birds.o $(LDFLAGS)

sema : rthread.o sema_use.o
	$(CC) $(CFLAGS) -o sema rthread.o sema_use.o $(LDFLAGS)

lock : rthread.o lock_use.o
	$(CC) $(CFLAGS) -o lock rthread.o lock_use.o $(LDFLAGS)

two : rthread.o two_threads.o
	$(CC) $(CFLAGS) -o two rthread.o two_threads.o $(LDFLAGS)

cp : rthread.o cons_prod_use.o
	$(CC) $(CFLAGS) -o cp rthread.o cons_prod_use.o $(LDFLAGS)

rthread.o : rthread.c rthread.h
	$(CC) $(CFLAGS) -c rthread.c

lock_use.o : lock_use.c
	$(CC) $(CFLAGS) -c lock_use.c

sema_use.o : sema_use.c
	$(CC) $(CFLAGS) -c sema_use.c

two_threads.o : two_threads.c
	$(CC) $(CFLAGS) -c two_threads.c

cons_prod_use.o : cons_prod_use.c
	$(CC) $(CFLAGS) -c cons_prod_use.c

birds.o : birds.c
	$(CC) $(CFLAGS) -c birds.c

gpus.o : gpus.c
	$(CC) $(CFLAGS) -c gpus.c

clean :
	rm -rf *.o sema lock two cp birds gpus
