
CC = gcc
CFLAGS = -g -Wall

default: msgq

msgq: main.o msgq.o zemaphore.o
	$(CC) $(CFLAGS) -o msgq main.o msgq.o zemaphore.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

msgq.o: msgq.c msgq.h
	$(CC) $(CFLAGS) -c msgq.c

zemaphore.o: zemaphore.c zemaphore.h
	$(CC) $(CFLAGS) -c zemaphore.c

clean:
	rm -f msgq *.o