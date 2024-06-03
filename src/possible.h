#ifndef POSSIBLE_H
#define POSSIBLE_H

#include "structEnum.h"
bool isboardInvalid(Square board[8][8]) ;
void possibleMoves(Square board[8][8]); 
GameState checkCondi(Square board[8][8]) ;//function to check whether a player is checked or not

GameState isgameFinished(Square board[8][8]);                      //check if the game is finished

#endif // !POSSIBLE_H
