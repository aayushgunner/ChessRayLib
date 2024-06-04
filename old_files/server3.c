#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "structEnum.h"
#include<fcntl.h>
#include "errno.h"

#define PORTNUM 8898

static void error(const char *msg) {
    perror(msg);
    exit(1);
}

int initialize_server(int port_address) {
    int sockfd, newsockfd;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("Error opening socket");

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port_address);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("Binding failed");

    listen(sockfd, 1);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0)
        error("Error on accept");

    close(sockfd);
    return newsockfd;
}
void *receiveMessages(void *arg) {
    int sockfd = *(int *)arg;
    int flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

    while (1) {
        Move move;
        int bytesRead = recv(sockfd, &move, sizeof(Move), 0);
        if (bytesRead > 0) {
            printf("Move received: %d %d %d %d %d %d\n",
                   move.marekoX, move.marekoY, move.marekoPiece,
                   move.maarnePiece, move.khaanePieceX, move.khaanePieceY);
        } else if (bytesRead < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
            error("Error receiving move");
        }
        usleep(100000); // Sleep for 100 milliseconds to avoid high CPU usage
    }
}
void *sendMessages(void *arg) {
    int sockfd = *(int *)arg;
    int flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

    while (1) {
        int a[6];
        printf("Enter your move (marekoX marekoY marekoPiece maarnePiece khaanePieceX khaanePieceY): ");
        if (scanf("%d%d%d%d%d%d", &a[0], &a[1], &a[2], &a[3], &a[4], &a[5]) == 6) {
            Move moveToSend = {a[0], a[1], a[2], a[3], a[4], a[5]};
            send(sockfd, &moveToSend, sizeof(Move), 0);
        }
    }
}

int main() {
    int sockfd = initialize_server(PORTNUM);
    pthread_t receiveThread, sendThread;
    pthread_create(&receiveThread, NULL, receiveMessages, (void *)&sockfd);
    pthread_create(&sendThread, NULL, sendMessages, (void *)&sockfd);
    pthread_join(receiveThread, NULL);
    pthread_join(sendThread, NULL);
    close(sockfd);
    return 0;
}
