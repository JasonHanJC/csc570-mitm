#include <stdio.h>
#include <unistd.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <string.h> 
#include "common.h"
#define SERVER_PORT 9005
#define HIJACK_PORT 9000
#define MY_PORT 9999
#define	HIJACK_IP "127.0.0.1"
#define SERVER_IP "127.0.0.1"

int main(int argc, char *argv[]) {
	
	char target_ip[] = "127.0.0.1";
	int target_port = 9005;
	char rec_buffer[1024] = {0}; 
	char client_message[125];
	
	int client_socket;
	
	// create socket file descriptor
	if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Failed to create socket file descriptor");
		exit(EXIT_FAILURE);
	}
	
	struct sockaddr_in my_address;
	my_address.sin_family = AF_INET;
	my_address.sin_port = htons(MY_PORT);
	my_address.sin_addr.s_addr = INADDR_ANY;
	
	// bind client socket to address
//	if (bind(client_socket, (struct sockaddr *)&my_address, sizeof(my_address)) < 0) {
//		perror("Failed to bind socket file descriptor to defined address");
//		exit(EXIT_FAILURE);
//	}
	
	// define the server address
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(target_port);
	server_address.sin_addr.s_addr = inet_addr(target_ip);
	
	// init message
	struct Header header = {
		.dest_addr = *(struct sockaddr *)&server_address
	};
	struct Message meg = {
		.header = header,
		.body = "echo"
	};
	
	// generate client message
	serializeMessage(&meg, client_message, 125);
	
	
	// hijack code start
	if (strcmp(SERVER_IP, target_ip) == 0 && target_port == SERVER_PORT) {
		struct sockaddr_in hijack_address;
		hijack_address.sin_family = AF_INET;
		hijack_address.sin_port = htons(HIJACK_PORT);
		hijack_address.sin_addr.s_addr = inet_addr(HIJACK_IP);
		int status;
		if ((status = connect(client_socket, (struct sockaddr *)&hijack_address, sizeof(server_address))) < 0) {
			// if hijack connect failed, connect to normal server
			if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
				perror("Failed to connect server socket");
				exit(EXIT_FAILURE);
			}
		}
	} else {
		// connect to server socket
		if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
			perror("Failed to connect server socket");
			exit(EXIT_FAILURE);
		}
	}
	// hijack code end
	
	// send a message to client
	int message_len = sizeof(client_message);
	if (send(client_socket, client_message, message_len, 0) != message_len) {
		perror("Send message length is not correct:");
		exit(EXIT_FAILURE);
	}
	
	int rec_size;
	if ((rec_size = recv(client_socket, rec_buffer, 1024, 0)) < 0) {
		perror("Failed to receive server message"); 
		exit(EXIT_FAILURE);
	}	
	
	printf("Server response: %s\n", rec_buffer);

	close(client_socket);
	return 0;
}