CC=g++
CFLAGS=-Wall

all: Server Client MITM
	
Server: Server.cpp
	$(CC) $(CFLAGS) Server.cpp -o Server
	
Client: Client.cpp
	$(CC) $(CFLAGS) Client.cpp -o Client
	
MITM: MITM.cpp
	$(CC) $(CFLAGS) MITM.cpp -o MITM
	
clean:
	rm -rf Server Client MITM
