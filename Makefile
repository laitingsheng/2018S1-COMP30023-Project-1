CC      = gcc
CFLAGS  = -O3 -Wall -Wextra -Wpedantic
EXE     = server
OBJ     = bin/main.o bin/core/httpd.o bin/core/server.o

all: mkdir $(EXE)

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^
bin/main.o: src/main.c
	$(CC) $(CFLAGS) -c -o $@ $^
bin/core/httpd.o: src/core/httpd.c
	$(CC) $(CFLAGS) -c -o $@ $^
bin/core/server.o: src/core/server.c
	$(CC) $(CFLAGS) -c -o $@ $^

mkdir:
	@mkdir -p bin/core

clean:
	@rm -rf bin
