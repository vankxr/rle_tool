CFLAGS = -O3 -Wall
CC = gcc

all: rle_encode rle_decode

rle_encode: rle_encode.c
	@$(CC) $(CFLAGS) $< -o $@

rle_decode: rle_decode.c
	@$(CC) $(CFLAGS) $< -o $@

install:
	@cp ./rle_encode /usr/local/bin
	@cp ./rle_decode /usr/local/bin

uninstall:
	@rm /usr/local/bin/rle_encode
	@rm /usr/local/bin/rle_decode

clean:
	@echo "RM *.o esptool2 esptool2.exe"
	@rm -f *.o
	@rm -f esptool2 esptool2.exe
