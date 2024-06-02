
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "structEnum.h"

void error (const char *msg) {
  perror(msg);
  exit(1);
}

int main (int argc , char * argv[]) {

  int sockfd,portno,n;
  struct sockaddr_in serv_addr;
  struct hostent *server;

  char buffer[255];
  if (argc <3 ) {

    fprintf(stderr, "usage %s hostname port \n", argv[0]);
    exit(1);

  }

  portno = atoi(argv[2]);
  sockfd = socket(AF_INET , SOCK_STREAM, 0);
  if (sockfd < 0) {
    error ("error opening circuit");

  }

  server = gethostbyname(argv[1]);
  if (server==NULL) {
    fprintf(stderr , "Error, no such host");
  }

  bzero(((char *) &serv_addr) , sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;

  bcopy((char *) server->h_addr , (char *)&serv_addr.sin_addr.s_addr, server->h_length);

  serv_addr.sin_port = htons(portno);
  if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    error("connection failed");
  }
  while (1) {
  // Create a move (for testing, you can modify it to get actual moves)
        int a[6];
        printf("Enter your move (marekoX marekoY marekoPiece maarnePiece khaanePieceX khaanePieceY): ");
        scanf("%d%d%d%d%d%d", &a[0], &a[1], &a[2], &a[3], &a[4], &a[5]);
        Move moveToSend = {a[0], a[1], a[2], a[3], a[4], a[5]};
        
        // Send the move to the server
        char buffer[sizeof(Move)];
        memcpy(buffer, &moveToSend, sizeof(Move));
        n = send(sockfd, buffer, sizeof(Move), 0);
        if (n < 0) {
            error("Error sending move");
        }

        // Clear the buffer and receive the move from the server
        bzero(buffer, sizeof(Move));
        Move receivedMove;
        n = recv(sockfd, buffer, sizeof(Move), 0);
        if (n < 0) {
            error("Error receiving move");
        }
        memcpy(&receivedMove, buffer, sizeof(Move));
        
        // Print the received move
        printf("Move received: %d %d %d %d %d %d\n", 
               receivedMove.marekoX, 
               receivedMove.marekoY, 
               receivedMove.marekoPiece, 
               receivedMove.maarnePiece, 
               receivedMove.khaanePieceX, 
               receivedMove.khaanePieceY);
    }

    close(sockfd);
    return 0;
//     Move haha = {1,2,3,4,5,6};
//     
//    char buffer[sizeof(Move)];
//    memcpy(buffer, &haha, sizeof(Move));
//     int n = send(sockfd , buffer , sizeof(Move), 0);
//
//         if (n < 0) {
//         error("Error sending move");
//     }
//     bzero(buffer, sizeof(Move));
//     Move sad;
//     n=recv(sockfd, buffer, sizeof(buffer), 0);
//     if (n<0) {
//       error("Error receiving moves");
//     }
//     memcpy(&sad, buffer, sizeof(Move));
//     printf("%d\n%d\n%d\n%d\n%d\n%d\n",sad.marekoX,sad.marekoY,sad.marekoPiece,sad.maarnePiece,sad.khaanePieceX,sad.khaanePieceY); 
// }
//
//
//
//
// close(sockfd);


  }







