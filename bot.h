#ifndef BOT_H
#define BOT_H
#include "structEnum.h"
int evalFunc(Square board[8][8]);
int miniMaxbot(Square board[8][8], Move temp,int depth) ;
void moveBot(Square board[8][8]) ;
#endif
