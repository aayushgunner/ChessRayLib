#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "structEnum.h"
#include <fcntl.h>
static void error(const char *msg) {
    perror(msg);
    exit(1);
}
int initialize_server(int Port_address){
    int sockfd, newsockfd;
    struct sockaddr_in client_addr;
    socklen_t clilen = sizeof(client_addr);

    // Start listening
    struct sockaddr_in serv_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("Error opening socket");

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(Port_address);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("Binding failed");

    listen(sockfd, 1); // Listen for up to 2 clients
    // Complete listening
    printf("Waiting for Client to connect...\n");
    newsockfd = accept(sockfd, (struct sockaddr *)&client_addr, &clilen);
    if (newsockfd < 0) error("Error on accept for player 1");
    printf("Conneted to client.\n");
    close(sockfd);
    return newsockfd;
}

void send_serverMove(int sockfd, Move move) {
    //  int flags = fcntl(sockfd, F_GETFL, 0);
    // fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
    char buffer[sizeof(Move)];
    memcpy(buffer, &move, sizeof(Move));
    if(send(sockfd, buffer, sizeof(Move), 0) < 0) error("Error sending move");
    printf("Move was sent by server\n");
}

Move receive_clientMove(int sockfd) {
    //  int flags = fcntl(sockfd, F_GETFL, 0);
    // fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
    char buffer[sizeof(Move)];
    if(recv(sockfd, buffer, sizeof(Move), 0) < 0) error("Error receiving move");

    Move move;
    memcpy(&move, buffer, sizeof(Move));
    printf("Move was received by server\n");
    return move;
}


void close_server(int sockfd){
    close(sockfd);
}
