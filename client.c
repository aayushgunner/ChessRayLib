#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "structEnum.h"

static void error(const char *msg) {
    perror(msg);
    exit(1);
}

int initialize_client(char *ip_address, int port_address){
    int sockfd;
    struct sockaddr_in serv_addr;
    struct hostent *server;


    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("Error opening socket");

    server = gethostbyname(ip_address);

    if (server == NULL) {
        fprintf(stderr, "Error, no such host\n");
        exit(1);
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(port_address);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        error("Connection failed");
    }
    printf("Connected to server\n");
    return sockfd;
}

void send_clientMove(int sockfd, Move moveToSend){
    char buffer[sizeof(Move)];
    memcpy(buffer, &moveToSend, sizeof(Move));
    if(send(sockfd, buffer, sizeof(Move), 0) < 0) error("Error sending move");
    printf("Move was sent by client\n");

}

Move receive_serverMove(int sockfd){
    char buffer[sizeof(Move)];
    bzero(buffer, sizeof(Move));
    Move receivedMove;
    if(recv(sockfd, buffer, sizeof(Move), 0) < 0) error("Error receiving move");
    memcpy(&receivedMove, buffer, sizeof(Move));

    printf("Move was received by client\n");
    return receivedMove;
}

void close_client(int sockfd){
    close(sockfd);
}
