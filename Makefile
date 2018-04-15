CC      = gcc
CFLAGS  = -O3 -Wall -Wextra -Wpedantic
EXE     = server
OBJ     = bin/main.o bin/core/file.o bin/core/httpd.o bin/core/server.o

# fork works better on single core, but still supports multithreading
$(EXE): mkdir $(OBJ)
	$(CC) $(CFLAGS) -lpthread -o $@ $(OBJ)
bin/main.o: src/main.c
	$(CC) $(CFLAGS) -c -o $@ $^
bin/core/file.o: src/core/file.c
	$(CC) $(CFLAGS) -c -o $@ $^
bin/core/httpd.o: src/core/httpd.c
	$(CC) $(CFLAGS) -c -o $@ $^
bin/core/server.o: src/core/server.c
	$(CC) $(CFLAGS) -lpthread -c -o bin/core/server.o src/core/server.c

mkdir:
	mkdir -p bin/core

.PHONY: clean

clean:
	rm -rf bin
	rm -f server
