CC = gcc
CFLAGS = -Wall -g
LIBS = -lSDL2 -lSDL2_image

cangkupan: main.c init.c draw.c structs.h
	$(CC) $(CFLAGS) main.c init.c draw.c -o cangkupan $(LIBS)
clean:
	rm -rf cangkupan
