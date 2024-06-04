#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
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

int initialize_client(char *ip_address, int port_address) {
    int sockfd;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("Error opening socket");

    server = gethostbyname(ip_address);
    if (server == NULL) {
        fprintf(stderr, "Error, no such host\n");
        exit(1);
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(port_address);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("Connection failed");

    return sockfd;
}

void *receiveMessages(void *arg) {
    int sockfd = *(int *)arg;
    while (1) {
        Move move;
        if (recv(sockfd, &move, sizeof(Move), 0) > 0) {
            printf("Move received: %d %d %d %d %d %d\n",
                   move.marekoX, move.marekoY, move.marekoPiece,
                   move.maarnePiece, move.khaanePieceX, move.khaanePieceY);
        }
    }
}

void *sendMessages(void *arg) {
    int sockfd = *(int *)arg;
    while (1) {
        int a[6];
        printf("Enter your move (marekoX marekoY marekoPiece maarnePiece khaanePieceX khaanePieceY): ");
        scanf("%d%d%d%d%d%d", &a[0], &a[1], &a[2], &a[3], &a[4], &a[5]);
        Move moveToSend = {a[0], a[1], a[2], a[3], a[4], a[5]};
        send(sockfd, &moveToSend, sizeof(Move), 0);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <ip_address>\n", argv[0]);
        exit(1);
    }

    int sockfd = initialize_client(argv[1], 8898);

    pthread_t receiveThread, sendThread;
    pthread_create(&receiveThread, NULL, receiveMessages, (void *)&sockfd);
    pthread_create(&sendThread, NULL, sendMessages, (void *)&sockfd);

    pthread_join(receiveThread, NULL);
    pthread_join(sendThread, NULL);

    close(sockfd);
    return 0;
}
