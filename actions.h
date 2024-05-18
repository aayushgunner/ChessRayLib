#ifndef ACTIONS_H
#define ACTIONS_H

#include "structEnum.h"
#include "globals.h"
#
void undoMove (Square board[8][8], Move backGo) ;
void initializeArray (Vector availableMoves) ;
void addMoves (Move characs ); 
void initializeBoard(Square board[8][8]);
Texture2D * textureLoad(char *imagePaths[]);
void initializePiece(Square board[8][8]);
void promotionPiece(Square board[8][8]);

void performMove(Square board[8][8],int selectedY,int selectedX,int x,int y);
Move *getCopy() ;

#endif
