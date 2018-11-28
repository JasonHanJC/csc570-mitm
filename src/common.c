#include <stdio.h>
#include <unistd.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <arpa/inet.h>
#include <netinet/in.h> 
#include <string.h> 
#include "common.h"


void serializeMessage(struct Message *meg, char *buffer, int bufferLen) {
	int dest_port;
	char addr[INET_ADDRSTRLEN];
	
	struct sockaddr_in *s = (struct sockaddr_in *)&meg->header.dest_addr;
	dest_port = ntohs(s->sin_port);
	inet_ntop(AF_INET, &s->sin_addr, addr, sizeof(addr));
	
	snprintf(buffer, bufferLen, "%s#%s#%d", meg->body,
		addr,
		dest_port);
}

void deserializeMessage(const char *buffer, struct Message *meg) {
	int dest_port;
	char addr[INET_ADDRSTRLEN];
	
	sscanf(buffer, "%99[^#]%*c %15[^#]%*c %d", meg->body, addr, &dest_port);
	
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(dest_port);
	address.sin_addr.s_addr = inet_addr(addr);
	
	meg->header.dest_addr = *(struct sockaddr *)&address;
}

void printMessage(const char *prefix, struct Message *meg) {
	int dest_port;
	char addr[INET_ADDRSTRLEN];
	
	struct sockaddr_in *s = (struct sockaddr_in *)&meg->header.dest_addr;
	dest_port = ntohs(s->sin_port);
	inet_ntop(AF_INET, &s->sin_addr, addr, sizeof(addr));
	
	printf("%s { \n\tBody: %s \n\tDestination address: %s:%d\n}\n",prefix, meg->body, addr, dest_port);
}


