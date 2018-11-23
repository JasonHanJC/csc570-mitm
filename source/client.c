#include <stdio.h>
#include <unistd.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <string.h> 
#define PORT 8080
#define SERVER_IP "127.0.0.1"

int main(int argc, char *argv[]) {
	
	char client_message[5] = "echo";
	char rec_buffer[1024] = {0}; 
	
	int client_socket;
	
	// create socket file descriptor
	if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Failed to create socket file descriptor");
		exit(EXIT_FAILURE);
	}
	
	// define the server address
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(PORT);
	// server is running on localhost 127.0.0.1
	server_address.sin_addr.s_addr = inet_addr(SERVER_IP);
	
	// connect to server socket
	if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
		perror("Failed to connect server socket:");
		exit(EXIT_FAILURE);
	}
	
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