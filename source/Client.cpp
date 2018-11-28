#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
using namespace std;

#define SERVER_IP "127.0.0.1"
#define MITM_IP "0.0.0.0"

#define SERVER_PORT 8080
#define MITM_PORT 8081

int main(int argc, char *argv[]) {
    
    while(1)
    {
        int client_socket, server_port;
        cout<<"please input the message you want to send to the server"<<endl;
        string msg, input_server_ip;
        cin>>msg;
        cout<<"please input the server ip you want to send the message to"<<endl;
        cin>>input_server_ip;
        char rec_buffer[1024] = {0};
        if(input_server_ip.compare(SERVER_IP) == 0){
            input_server_ip = MITM_IP;
            server_port = MITM_PORT;
        }
        
        //input_server_ip = SERVER_IP;
        //server_port = 8080;
        
        // create socket file descriptor
        if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            perror("Failed to create socket file descriptor");
            exit(EXIT_FAILURE);
        }
        
        // define the server address
        struct sockaddr_in server_address;
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(server_port);
        // server is running on localhost 127.0.0.1
        server_address.sin_addr.s_addr = inet_addr(input_server_ip.c_str());
        
        // connect to server socket
        if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
            perror("Failed to connect server socket:");
            exit(EXIT_FAILURE);
        }
        
        // send a message to server
        int message_len = sizeof(msg.c_str());
        if (send(client_socket, msg.c_str(), message_len, 0) != message_len) {
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
    }
    
    return 0;
}
