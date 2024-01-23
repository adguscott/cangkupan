CC = gcc
CFLAGS = -Wall -g
LIBS = -lSDL2

cangkupan:
	$(CC) $(CFLAGS) main.c init.c -o cangkupan $(LIBS)
clean:
	rm -rf cangkupan
