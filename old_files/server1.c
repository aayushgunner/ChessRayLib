#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "structEnum.h"

#define PORTNUM 8898

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

    printf("Waiting for player 1 to connect...\n");
    newsockfd = accept(sockfd, (struct sockaddr *)&client_addr, &clilen);
    if (newsockfd < 0) error("Error on accept for player 1");
    printf("Player 1 connected.\n");
    close(sockfd);
    return newsockfd;
}

void send_serverMove(int sockfd, Move move) {
    char buffer[sizeof(Move)];
    memcpy(buffer, &move, sizeof(Move));
    if(send(sockfd, buffer, sizeof(Move), 0) < 0) error("Error sending move");
}

Move receive_clientMove(int sockfd) {
    char buffer[sizeof(Move)];
    if(recv(sockfd, buffer, sizeof(Move), 0) < 0) error("Error receiving move");

    Move move;
    memcpy(&move, buffer, sizeof(Move));
    return move;
}


void close_server(int sockfd){
    close(sockfd);
}

int main() {
    int sockfd = initialize_server(PORTNUM);
    int a[6];
    while (1) {

        Move move1 = receive_clientMove(sockfd);
        printf("Move received: %d %d %d %d %d %d\n",
               move1.marekoX,
               move1.marekoY,
               move1.marekoPiece,
               move1.maarnePiece,
               move1.khaanePieceX,
               move1.khaanePieceY);

        printf("Enter your move (marekoX marekoY marekoPiece maarnePiece khaanePieceX khaanePieceY): ");
        scanf("%d%d%d%d%d%d", &a[0], &a[1], &a[2], &a[3], &a[4], &a[5]);
        Move moveToSend = {a[0], a[1], a[2], a[3], a[4], a[5]};
        send_serverMove(sockfd, moveToSend);
    }

    close_server(sockfd);
    return 0;
}
