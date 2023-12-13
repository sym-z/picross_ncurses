CC = gcc
CFLAGS = -g -O3 -Wall -Wpedantic -Wextra
OBJS = clueq.o non.o main.o 
LIBS = -lncurses

a.out: $(OBJS) 
	$(CC) $(CFLAGS) $(OBJS) $(LIBS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

non.o: non.c non.h
	$(CC) $(CFLAGS) -c non.c

clueq.o: clueq.c clueq.h
	$(CC) $(CFLAGS) -c clueq.c

clean:
	rm -rf *.o a.out
