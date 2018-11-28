CC=g++
CFLAGS=-Wall

all: server client proxy

common.o:  common.c common.h 
	$(CC) $(CFLAGS) -c common.c
	
server: common.o
	$(CC) $(CFLAGS) server.c common.o -o server
	
client: common.o
	$(CC) $(CFLAGS) client.c common.o -o client
	
proxy: common.o
	$(CC) $(CFLAGS) mitm-proxy.c common.o -o mitm-proxy
	
clean:
	rm -rf server client mitm-proxy common.o
