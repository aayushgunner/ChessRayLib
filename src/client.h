#ifndef CLIENT_H
#define CLIENT_H
#include "structEnum.h"
#include "globals.h"

int initialize_client(char *ip_address, int port_address);
void send_clientMove(int sockfd, Move moveToSend);
Move receive_serverMove(int sockfd);
void close_client(int sockfd);

#endif // !CLIENT_H
