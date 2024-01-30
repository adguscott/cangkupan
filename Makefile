CC = gcc
CFLAGS = -Wall -g
LIBS = -lSDL2 -lSDL2_image

cangkupan: main.c init.c draw.c defs.h physics.c structs.h
	$(CC) $(CFLAGS) main.c init.c draw.c physics.c -o cangkupan $(LIBS)
clean:
	rm -rf cangkupan
