CC=g++
CFLAGS=-Wall

all: server client

server:
	$(CC) $(CFLAGS) server.c -o server
	
client:
	$(CC) $(CFLAGS) client.c -o client
	
clean:
	rm -rf server client
