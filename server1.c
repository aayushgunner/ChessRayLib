
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "structEnum.h"

#define PORTNUM 8898

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int start_listening() {
    int sockfd;
    struct sockaddr_in serv_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("Error opening socket");

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORTNUM);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("Binding failed");

    listen(sockfd, 1); // Listen for up to 2 clients
    return sockfd;
}

void sendMove(int sockfd, Move move) {
    char buffer[sizeof(Move)];
    memcpy(buffer, &move, sizeof(Move));
    int n = send(sockfd, buffer, sizeof(Move), 0);
    if (n < 0) error("Error sending move");
}

Move receiveMove(int sockfd) {
    char buffer[sizeof(Move)];
    int n = recv(sockfd, buffer, sizeof(Move), 0);
    if (n < 0) error("Error receiving move");

    Move move;
    memcpy(&move, buffer, sizeof(Move));
    return move;
}

int main() {
    int sockfd, newsockfd1;
    struct sockaddr_in client_addr;
    socklen_t clilen = sizeof(client_addr);

    sockfd = start_listening();

    printf("Waiting for player 1 to connect...\n");
    newsockfd1 = accept(sockfd, (struct sockaddr *)&client_addr, &clilen);
    if (newsockfd1 < 0) error("Error on accept for player 1");
    printf("Player 1 connected.\n");
    int a[6];
   int n;
    while (1) {
    
        Move move1 = receiveMove(newsockfd1);
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
        sendMove(newsockfd1, moveToSend);
    }

    close(newsockfd1);
    close(sockfd);
    return 0;
}
