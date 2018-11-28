#include <stdio.h>
#include <unistd.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <string.h> 
#include "common.h"
#define PORT 9005

int main(int argc, char *argv[]) {
	
	char rec_buffer[1024] = {0}; 
	
	int server_socket;
	
	// create socket file descriptor
	if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("Failed to create socket file descriptor.");
		exit(EXIT_FAILURE);
	}
	
	// define the server address
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(PORT);
	server_address.sin_addr.s_addr = INADDR_ANY;
	
	// bind the socket to address
	if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
		perror("Failed to bind socket file descriptor to defined address.");
		exit(EXIT_FAILURE);
	}
	
	// listen the socket, max pending connection 3
	if (listen(server_socket, 3) < 0) { 
		perror("Failed to listen the socket"); 
		exit(EXIT_FAILURE); 
	} 
	
	printf("Server is running on 127.0.0.1:9005 \n");

	for (;;) {
		// establish a new socket for data-transfer
		int client_socket;
		struct sockaddr_in client_address;
		int addresslen = sizeof(client_address);
		
		if ((client_socket = accept(server_socket, (struct sockaddr *)&client_address, (socklen_t *)&addresslen)) < 0) {
			perror("Failed to accept client socket"); 
			exit(EXIT_FAILURE);
		}
		
		int rec_size;
		if ((rec_size = recv(client_socket, rec_buffer, 1024, 0)) < 0) {
			perror("Failed to receive client message"); 
			exit(EXIT_FAILURE);
		}		
		
		
		while (rec_size > 0) {
			
			// deserialize the destination address
			struct Message rec_meg;
			deserializeMessage(rec_buffer, &rec_meg);
			printMessage(&rec_meg);
			
			int send_size = rec_size;
			char send_mesg[send_size];
			send_size = rec_size + 3;
			// add $$$ in front client message
			strcpy(send_mesg, "$$$");
			strncat (send_mesg, rec_buffer, rec_size);
						
			if (send(client_socket, send_mesg, send_size, 0) != send_size) {
				perror("Failed to send client message"); 
				exit(EXIT_FAILURE);
			}
			if ((rec_size = recv(client_socket, rec_buffer, 1024, 0)) < 0) {
				perror("Failed to receive client message"); 
				exit(EXIT_FAILURE);
			}	
		}
		
		close(client_socket);
	}
	
	close(server_socket);
	
	return 0;
}