################################################################################
# Makefile for the project
# Create by Tingsheng Lai (tingshengl, 781319)
################################################################################

CC      = gcc
CFLAGS  = -O3 -std=c99 -Wall -Wextra -Wpedantic
EXE     = server
OBJ     = bin/main.o bin/core/file.o bin/core/httpd.o bin/core/server.o

$(EXE): mkdir $(OBJ)
	$(CC) $(CFLAGS) -pthread -o $@ $(OBJ)
bin/main.o: src/main.c
	$(CC) $(CFLAGS) -c -o $@ $^
bin/core/file.o: src/core/file.c
	$(CC) $(CFLAGS) -c -o $@ $^
bin/core/httpd.o: src/core/httpd.c
	$(CC) $(CFLAGS) -c -o $@ $^
bin/core/server.o: src/core/server.c
	$(CC) $(CFLAGS) -pthread -c -o bin/core/server.o src/core/server.c

mkdir:
	mkdir -p bin/core

.PHONY: clean

clean:
	rm -rf bin
	rm -f server
