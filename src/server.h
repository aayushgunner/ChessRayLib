#ifndef SERVER_H
#define SERVER_H
#include "structEnum.h"
#include "globals.h"

int initialize_server(int Port_address);
void send_serverMove(int sockfd, Move move);
bool receive_clientMove(int sockfd, Move * receivedMove);
void close_server(int sockfd);

#endif // !SERVER_H
