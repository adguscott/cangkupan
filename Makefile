CC = gcc
CFLAGS = -Wall -g
LIBS = -lSDL2

cangkupan:
	$(CC) $(CFLAGS) main.c -o cangkupan $(LIBS)
clean:
	rm -rf cangkupan
