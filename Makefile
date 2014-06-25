P=flatwm.exe
CC=gcc
CFLAGS = -g -Wall -m64 -std=c11 -mwindows

all:
	$(CC) $(CFLAGS) -o $(P) src/*.c
