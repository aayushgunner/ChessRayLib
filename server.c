#include "structEnum.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define portnum "8898"
void error (const char *msg) {
  perror(msg);
  exit(1);
}


int start_listening() {


  int sockfd ,newsockfd,  n , portno;
  struct sockaddr_in serv_addr, client_addr;
  socklen_t clilen;

  sockfd = socket(AF_INET, SOCK_STREAM,  0);
  if (sockfd<0) {
    error("Error opening socket");
  }

  bzero((char *)&serv_addr, sizeof(serv_addr));
  portno = atoi(portnum);
  serv_addr.sin_family=AF_INET;
  serv_addr.sin_addr.s_addr=INADDR_ANY;
  serv_addr.sin_port=htons(portno);

  if (bind(sockfd, (struct sockaddr *)&serv_addr,sizeof(serv_addr))<0) {
    error("Binding failed");

  }
  listen(sockfd, 2);
  clilen = sizeof(client_addr);
  newsockfd = accept(sockfd, (struct sockaddr *)&client_addr, &clilen);
  if (newsockfd < 0) {
    error("Error on accept");
  }

  return newsockfd;
}


void sendMoves(int sockfd , Move move) {
  char buffer[sizeof(Move)];
  memcpy(buffer, &move, sizeof(Move));
  int n = send(sockfd, buffer, sizeof(Move), 0);
    if (n < 0) {
        error("Error sending move");
    }
}


Move receiveMoves(int sockfd) {

  char buffer[sizeof(Move)];
  int n = recv(sockfd, buffer, sizeof(Move), 0);
  if (n < 0) {
        error("Error receiving move");
    }
    Move move;
    memcpy(&move, buffer, sizeof(Move));
    return move;
}

int main() {
//   int sockfd = start_listening();
//
// while (1) {
//       Move sad = receiveMoves(sockfd);
//       printf("%d\n%d\n%d\n%d\n%d\n%d\n",sad.marekoX,sad.marekoY,sad.marekoPiece,sad.maarnePiece,sad.khaanePieceX,sad.khaanePieceY); 
//     int a[6];
//       scanf("%d%d%d%d%d%d", &a[0],&a[1],&a[2],&a[3],&a[4],&a[5] );
//       Move lyang = {a[0],a[1],a[2],a[3],a[4],a[5]};
//       sendMoves(sockfd, lyang);
//   }
  int sockfd, newsockfd1, newsockfd2;
    struct sockaddr_in client_addr;
    socklen_t clilen = sizeof(client_addr);
    
    sockfd = start_listening();
    
    printf("Waiting for player 1 to connect...\n");
    newsockfd1 = accept(sockfd, (struct sockaddr *)&client_addr, &clilen);
    if (newsockfd1 < 0) error("Error on accept for player 1");
    printf("Player 1 connected.\n");
    
    printf("Waiting for player 2 to connect...\n");
    newsockfd2 = accept(sockfd, (struct sockaddr *)&client_addr, &clilen);
    if (newsockfd2 < 0) error("Error on accept for player 2");
    printf("Player 2 connected.\n");
    
    while (1) {
        Move move1 = receiveMoves(newsockfd1);
        sendMoves(newsockfd2, move1);

        Move move2 = receiveMoves(newsockfd2);
        sendMoves(newsockfd1, move2);
    }
    
    close(newsockfd1);
    close(newsockfd2);
    close(sockfd);
    return 0;
}
