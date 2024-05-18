
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

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

  bzero(buffer, 255);
  while (1) {
    fgets(buffer , 255 , stdin);
    n=send(sockfd , buffer , strlen(buffer),0);

    if (n<0) {

      error ("Error on writing");
    }


    bzero(buffer , 255);
    n=recv(sockfd, buffer, 255,0);
    if (n<0) {
      error("Error on reading");

    }

    printf("Server: %s", buffer);
    int i= strncmp("Bye", buffer, 3);
    if (i==0) {
      break;
    }



  }

  close (sockfd);
  return 0;

  






}

