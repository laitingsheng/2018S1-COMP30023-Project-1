CC      = gcc
CFLAGS  = -O3 -Wall -Wextra -Wpedantic
EXE     = server
OBJ     = bin/main.o bin/server/httpd.o bin/server/server.o

all: mkdir $(EXE)

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^
bin/main.o: src/main.c
	$(CC) $(CFLAGS) -c -o $@ $^
bin/server/httpd.o: src/server/httpd.c
	$(CC) $(CFLAGS) -c -o $@ $^
bin/server/server.o: src/server/server.c
	$(CC) $(CFLAGS) -c -o $@ $^

mkdir:
	@mkdir -p bin/server

clean:
	@rm -rf bin
