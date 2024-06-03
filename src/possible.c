#include "possible.h"
#include "actions.h"
#include "globals.h"
#include "moves.h"

bool isboardInvalid(Square board[8][8]) {
  return ((checkCondi(board)==blackChecked && someCheck==1)|| (checkCondi(board)==whiteChecked && someCheck==0)|| checkCondi(board)==inValid);
}


void possibleMoves(Square board[8][8]) {
  availableMoves.size=0;
  for (int i=0;i<8;i++) {
    for (int j=0;j<8;j++) {
      if (board[i][j].base!=EMPTY && board[i][j].base%2==someCheck) {
        for (int a=0;a<8;a++) {
          for (int b=0;b<8;b++) {
            if (canMove(board, j, i, WHITE, b, a) && (board[a][b].base==EMPTY || board[i][j].base%2!=board[a][b].base%2)  ) {
              Move temp;
              temp.marekoX=b;
              temp.marekoY=a;
              temp.khaanePieceX=j;
              temp.khaanePieceY=i;
              temp.marekoPiece=board[a][b].base;
              temp.maarnePiece=board[i][j].base;                                    //calcualte all the possible moves
              performMove(board, i, j, b, a);
              if (isboardInvalid(board)) {
                undoMove(board, temp);
                continue;
              }
              undoMove(board,temp);
              addMoves(temp);

            }
          }
        }
      }
    }
  }
  // printf("%lu size \n",availableMoves.size);
}


GameState checkCondi(Square board[8][8]) {                      //function to check whether a player is checked or not
  int blackKingX;
  int blackKingY;
  int whiteKingX;
  int whiteKingY;
  bool isBlack=false;
  bool isWhite=false;
  for (int i=0;i<8;i++) {
    for (int j=0;j<8;j++) {                              
      if (board[i][j].base==KINGB) {
        blackKingX=j;
        blackKingY=i;
      }
      if (board[i][j].base==KINGW) {
        whiteKingX=j;
        whiteKingY=i;
      }
    }
  }
  for (int i=0;i<8;i++) {
    for (int j=0;j<8;j++) {
      if (board[i][j].base!=EMPTY){
        if (board[i][j].base%2==0) {
          if (canMove(board, j, i, WHITE, whiteKingX , whiteKingY)) {
            // printf("WHite is checked");
            isWhite=true;
            // return whiteChecked;
          }
        }
        else  {
          if (canMove(board, j, i, WHITE, blackKingX, blackKingY)) {
            // printf("Black is checked");
            isBlack=true;
            // return blackChecked;
          }
        }
      }
    }
  }
  if (isWhite && isBlack) {
    return inValid;
  }

  else if (isWhite) {
    return whiteChecked;
  }

  else if (isBlack) {
    return blackChecked;
  }
  return playOn;
}



GameState isgameFinished(Square board[8][8]) {                         //check if the game is finished
  if (checkCondi(board)==whiteChecked) {
    possibleMoves(board);
    if (availableMoves.size==0) {
      return blackWins;
    }
  }
  else if (checkCondi(board)==blackChecked) {
    possibleMoves(board);
    if (availableMoves.size==0) {
      return whiteWins;
    }
  }
  return playOn;
}


