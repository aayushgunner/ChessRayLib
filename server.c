#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error (const char *msg) {
  perror(msg);
  exit(1);
}


int main(int argc , char * argv[]) {
 
  if (argc < 2) {
    fprintf(stderr , "Port no not provided. Program terminated\n");
    exit(1);
  }

  int sockfd , newsockfd , portno, n;
  char buffer[255];
  struct sockaddr_in serv_addr, client_addr;
  socklen_t clilen;
  
  sockfd = socket(AF_INET, SOCK_STREAM,  0);
  if (sockfd<0) {
    error("Error opening socket");
  }

  bzero((char *)&serv_addr, sizeof(serv_addr));
  portno = atoi(argv[1]);
  serv_addr.sin_family=AF_INET;
  serv_addr.sin_addr.s_addr=INADDR_ANY;
  serv_addr.sin_port=htons(portno);

  if (bind(sockfd, (struct sockaddr *)&serv_addr,sizeof(serv_addr))<0) {
    error("Binding failed");
    
  }

  listen(sockfd,5);
  clilen = sizeof(client_addr);
  newsockfd = accept(sockfd, (struct sockaddr *)&client_addr, &clilen);

  if (newsockfd < 0) {
    error("error on accept");
  }

  while (1) {
    bzero(buffer , 255);
    n=recv(newsockfd, buffer, 255, 0) ;
    if (n<0) {
      error("Error on reading");
    }
    printf("Client : %s \n", buffer);
    bzero(buffer , 255);

    fgets(buffer , 255,stdin);

    n=send(newsockfd,buffer,strlen(buffer),0);
    if (n<0) {
      error("Error on Writing");
    }
    int i= strncmp("Bye", buffer, 3);
    if (i==0) {
      break;
    }

  }
  close(newsockfd);
  close(sockfd);
  return 0;
  
}
