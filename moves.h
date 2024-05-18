#ifndef MOVES_H
#define MOVES_H
#include "headers.h"
#include "structEnum.h"

bool canMove(Square board[8][8],int selectedX,int selectedY, Color selectedColor , int x, int y) ;
bool pawnMoves(Square board[8][8],int selectedX, int selectedY,int x,int y );
bool rookMoves(Square board[8][8],int selectedX, int selectedY,int x, int y );
bool knightMoves(Square board[8][8],int selectedX, int selectedY,int x, int y);
bool bishopMoves(Square board[8][8],int selectedX, int selectedY,int x, int y  );
bool queenMoves(Square board[8][8],int selectedX, int selectedY, int x, int y );
bool kingMoves(Square board[8][8],int selectedX, int selectedY , int x, int y);
#endif // !MOVES_H

