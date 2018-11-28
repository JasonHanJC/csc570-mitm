#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
//using namespace std;
#define PORT 8081
#define MITM_IP "0.0.0.0"

#define SERVER_PORT 8080
#define SERVER_IP "127.0.0.1"

int main(int argc, char *argv[]) {
    
    char rec_buffer[1024] = {0};
    char rec_buffer_server[1024] = {0};
    
    int mitm_socket;
    
    // create socket file descriptor
    if ((mitm_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Failed to create socket file descriptor.");
        exit(EXIT_FAILURE);
    }
    
    // define the server address
    struct sockaddr_in mitm_address;
    mitm_address.sin_family = AF_INET;
    mitm_address.sin_port = htons(PORT);
    mitm_address.sin_addr.s_addr = inet_addr(MITM_IP);
    
    // binf the socket to address
    if (bind(mitm_socket, (struct sockaddr *)&mitm_address, sizeof(mitm_address)) < 0) {
        perror("Failed to bind socket file descriptor to defined address.");
        exit(EXIT_FAILURE);
    }
    
    // listen the socket, max pending connection 3
    if (listen(mitm_socket, 3) < 0) {
        perror("Failed to listen the socket");
        exit(EXIT_FAILURE);
    }
    
    int client_socket;
    struct sockaddr_in client_address;
    int addresslen = sizeof(client_address);

    struct sockaddr_in server_address;
    
    bool initializeMode = true;
    bool singleRound = true;
    int mitm_socket_to_server;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    // server is running on localhost 127.0.0.1
    server_address.sin_addr.s_addr = inet_addr(SERVER_IP);
    for (;;) {
        // establish a new socket for data-transfer
        
        if(initializeMode){
            if(singleRound)
            {
                if ((client_socket = accept(mitm_socket, (struct sockaddr *)&client_address, (socklen_t *)&addresslen)) < 0) {
                    perror("Failed to accept client socket");
                    //exit(EXIT_FAILURE);
                }
            }else{
                initializeMode = false;
//                if ((server_socket = accept(mitm_socket, (struct sockaddr *)&server_address, (socklen_t *)&addresslenServer)) < 0) {
//                    perror("Failed to accept client socket");
//                    exit(EXIT_FAILURE);
//                }
            }
        }
        
        int rec_size;
        if(singleRound){
            if ((rec_size = recv(client_socket, rec_buffer, 1024, 0)) < 0) {
                perror("Failed to receive client message");
                //exit(EXIT_FAILURE);
            }
        }
        else {
            rec_size = recv(mitm_socket_to_server, rec_buffer_server, 1024, 0);
        }
        
        
        
        
        if(initializeMode)
        {
            // create socket file descriptor
            if ((mitm_socket_to_server = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
                perror("Failed to create socket file descriptor.");
                //exit(EXIT_FAILURE);
            }
        
        
        
        
            // connect to server socket
            if (connect(mitm_socket_to_server, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
                perror("Failed to connect server socket:");
                //exit(EXIT_FAILURE);
            }
        }
        
        
        
        /*if ((
        if(sizeof(rec_size) <=0 ){
            rec_size = recv(server_socket, rec_buffer_server, 1024, 0);
        }) < 0) {
            perror("Failed to receive client message");
            exit(EXIT_FAILURE);
        }*/
        
        while (rec_size > 0) {
            int send_size = rec_size;
            std::string send_mesg(rec_buffer);
            if(singleRound){
                std::cout<<"intercept client mesasge: "<<rec_buffer<<std::endl;
                std::cout<<"Please select your operation: 1. Forward 2. Not forward 3. Modify"<<std::endl;
                bool repeat = true;
                int op;
                while(repeat){
                    std::cin>>op;
                    if(op ==1 || op == 2|op==3)
                    {
                        repeat = false;
                    }
                    else
                    {
                        std::cout<<"option invalid; please re-input!"<<std::endl;
                    }
                }
                if(op == 2){
                    rec_size = 0;
                    close(client_socket);
                    close(mitm_socket_to_server);
                    initializeMode = true;
                    singleRound = false;
                    continue;
                }
                if(op == 3){
                    std::cout<<"please input the message you want to change to"<<std::endl;
                    std::string s;
                    std::cin>>s;
                    send_mesg = s.c_str();
                    send_size = s.length();
                }
                // add $$$ in front client message
                //strcpy(send_mesg, "!!!");
                //strncat (send_mesg, rec_buffer, rec_size);
                
                if (send(mitm_socket_to_server, send_mesg.c_str(), send_size, 0) != send_size) {
                    perror("Failed to send server message");
                    exit(EXIT_FAILURE);
                }
                
//                if (send(client_socket, send_mesg.c_str(), send_size, 0) != send_size) {
//                    perror("Failed to send client message");
//                    exit(EXIT_FAILURE);
//                }
                
//                if ((rec_size = recv(client_socket, rec_buffer, 1024, 0)) < 0) {
//                    perror("Failed to receive client message");
//                    exit(EXIT_FAILURE);
//                }
                rec_size = 0;
                
            }
            else
            {
                /*if (send(client_socket, send_mesg.c_str(), send_size, 0) != send_size) {
                    perror("Failed to send client message");
                    exit(EXIT_FAILURE);
                }
                
                if ((rec_size = recv(client_socket, rec_buffer, 1024, 0)) < 0) {
                    perror("Failed to receive client message");
                    exit(EXIT_FAILURE);
                }*/
                std::cout<<"intercept server mesasge: "<<rec_buffer_server<<std::endl;
                std::cout<<"Please select your operation: 1. Forward 2. Not forward 3. Modify"<<std::endl;
                bool repeat = true;
                int op;
                while(repeat){
                    std::cin>>op;
                    if(op ==1 || op == 2|op==3)
                    {
                        repeat = false;
                    }
                    else
                    {
                        std::cout<<"option invalid; please re-input!"<<std::endl;
                    }
                }
                if(op == 2){
                    rec_size = 0;
                    close(client_socket);
                    close(mitm_socket_to_server);
                    initializeMode = true;
                    singleRound = false;
                    continue;
                }
                if(op == 3){
                    std::cout<<"please input the message you want to change to"<<std::endl;
                    std::string s;
                    std::cin>>s;
                    send_mesg = s.c_str();
                    send_size = s.length();
                }
                // add $$$ in front client message
                //strcpy(send_mesg, "!!!");
                //strncat (send_mesg, rec_buffer, rec_size);
                
                if (send(client_socket, send_mesg.c_str(), send_size, 0) != send_size) {
                    perror("Failed to send server message");
                    exit(EXIT_FAILURE);
                }
                rec_size = 0;
                close(client_socket);
                close(mitm_socket_to_server);
                initializeMode = true;
            }
        }
        singleRound = !singleRound;
        //initializeMode = false;
        //close(client_socket);
    }
    
    close(mitm_socket);
    
    return 0;
}
