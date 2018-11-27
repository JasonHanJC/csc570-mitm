#include <stdio.h>
#include <unistd.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include "common.h"
#define PORT 9000
#define NOMODIFY 0 // Forward as is
#define IGNORE 1 // Do not forward
#define MODIFY 2 // Modify before forwarding

int main(int argc, char *argv[]) {	
	if (argc > 2) {
		perror("Argument number error");
		exit(EXIT_FAILURE);
	}
	
	int mode = NOMODIFY;
	if (argc == 2) {
		mode = atoi(argv[1]);	
	}
	
	printf("MITM mode : %d\n", mode);
	char rec_buffer[1024] = {0}; 
	int mitm_socket_from_sender, mitm_socket_to_dest;
	
	// create socket file descriptor
	if ((mitm_socket_from_sender = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("Failed to create mitm_socket_from_sender file descriptor.");
		exit(EXIT_FAILURE);
	}
	
	// define the server address
	struct sockaddr_in mitm_address;
	mitm_address.sin_family = AF_INET;
	mitm_address.sin_port = htons(PORT);
	mitm_address.sin_addr.s_addr = INADDR_ANY;
	
	// binf the socket to address
	if (bind(mitm_socket_from_sender, (struct sockaddr *)&mitm_address, sizeof(mitm_address)) < 0) {
		perror("Failed to bind socket file descriptor to defined address.");
		exit(EXIT_FAILURE);
	}
	
	// listen the socket, max pending connection 3
	if (listen(mitm_socket_from_sender, 3) < 0) { 
		perror("Failed to listen the socket"); 
		exit(EXIT_FAILURE); 
	} 
	
	printf("MITM is running on 127.0.0.1:9000 \n");

	for (;;) {
		// STORE SENDER INFO
		int sender_socket;
		struct sockaddr_in sender_address;
		int addresslen = sizeof(sender_address);
		
		if ((sender_socket = accept(mitm_socket_from_sender, (struct sockaddr *)&sender_address, (socklen_t *)&addresslen)) < 0) {
			perror("Failed to accept client socket"); 
			exit(EXIT_FAILURE);
		}
		
		int rec_size;
		if ((rec_size = recv(sender_socket, rec_buffer, 1024, 0)) < 0) {
			perror("Failed to receive client message"); 
			exit(EXIT_FAILURE);
		}		
		
		// deserialize the destination address
		struct Message rec_meg;
		deserializeMessage(rec_buffer, &rec_meg);
		printMessage(&rec_meg);
		

		int send_size = rec_size;
		char send_mesg[send_size];
		if (mode == MODIFY) {
			send_size = rec_size + 3;
			// add $$$ in front client message
			strcpy(send_mesg, "+++");
			strncat (send_mesg, rec_buffer, rec_size);
		} else {
			strcpy(send_mesg, rec_buffer);
		}
		
		
		if (mode == IGNORE) {
			send(sender_socket, send_mesg, sizeof(rec_buffer), 0);
		} else {
			// forward message to real server
			if ((mitm_socket_to_dest = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
				perror("Failed to create mitm_socket_to_dest file descriptor.");
				exit(EXIT_FAILURE);
			}
			
			if (connect(mitm_socket_to_dest, (struct sockaddr *)&rec_meg.header, sizeof(rec_meg.header)) < 0) {
				perror("Failed to connect destination socket");
				exit(EXIT_FAILURE);
			}
			
			
			if (send(mitm_socket_to_dest, send_mesg, send_size, 0) != send_size) {
				perror("Failed to send dest message"); 
				exit(EXIT_FAILURE);
			}

			if ((rec_size = recv(mitm_socket_to_dest, rec_buffer, 1024, 0)) < 0) {
				perror("Failed to receive dest message"); 
				exit(EXIT_FAILURE);
			}
			
			// send back to client
			send(sender_socket, rec_buffer, sizeof(rec_buffer), 0);
		}
		close(mitm_socket_to_dest);
		close(sender_socket);
	}

	printf("MITM close");
	close(mitm_socket_from_sender);
	
	return 0;
}