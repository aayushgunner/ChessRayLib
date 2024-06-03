#include "moves.h"
#include "globals.h"

bool canMove(Square board[8][8],int selectedX,int selectedY, Color selectedColor , int x, int y) {
  if (board[selectedY][selectedX].base==PAWNB || board[selectedY][selectedX].base==PAWNW) {
    return  pawnMoves(board,selectedX,selectedY, x, y);
  }
  if (board[selectedY][selectedX].base==ROOKW || board[selectedY][selectedX].base==ROOKB) {
    return rookMoves(board,selectedX,selectedY, x, y);
  }
  if (board[selectedY][selectedX].base==KNIGHTB || board[selectedY][selectedX].base==KNIGHTW) {
    return knightMoves(board,selectedX,selectedY, x, y);
  }
  if (board[selectedY][selectedX].base==BISHOPB || board[selectedY][selectedX].base==BISHOPW) {
    return bishopMoves(board,selectedX,selectedY,x, y);
  }
  if (board[selectedY][selectedX].base==QUEENB || board[selectedY][selectedX].base==QUEENW) {
    return queenMoves(board,selectedX,selectedY,x,y);
  }
  if (board[selectedY][selectedX].base==KINGB || board[selectedY][selectedX].base==KINGW) {
    return kingMoves(board,selectedX,selectedY,x,y);
  }
  return false;
}


bool pawnMoves(Square board[8][8],int selectedX, int selectedY, int x, int y  ) {
  if (board[selectedY][selectedX].base==PAWNB) {
    pawnNum=1;
  }
  else if (board[selectedY][selectedX].base==PAWNW) {
    pawnNum=-1;
  }
  if ((selectedY==1 || selectedY==6) && y==selectedY+pawnNum*2 && x==selectedX && (board[selectedY+pawnNum][x].base==EMPTY && board[selectedY+pawnNum*2][x].base==EMPTY)  ) {
    return true;
  }

  if (y==selectedY+pawnNum && x==selectedX && (board[y][x].base==EMPTY) || 
    (y==selectedY+pawnNum && (x==selectedX+pawnNum || x==selectedX-pawnNum) && board[y][x].base%2!=someCheck && board[y][x].base!=EMPTY)||
    (y==selectedY+pawnNum && ((x==selectedX && board[y][x].base==EMPTY)|| ((x==selectedX+pawnNum || x==selectedX-pawnNum)&&board[y][x].base%2!=someCheck && board[y][x].base!=EMPTY)) )
  ) {
    return true;
  }
  return false;
}
//to check if the rook move about to be performed is valid
bool rookMoves(Square board[8][8], int selectedX, int selectedY, int x, int y ) {
  if (x==selectedX ^ y==selectedY) {
    int xGo = (x > selectedX) ? 1 : (x < selectedX) ? -1 : 0;
    int yGo = (y > selectedY) ? 1 : (y < selectedY) ? -1 : 0;

    for (int i=1;i<8;i++) {
      if (x==selectedX + i*xGo && y==selectedY + i*yGo) {
        return true;
      }  
      else if (board[selectedY+i*yGo][selectedX+i*xGo].base!=EMPTY) {
        break;
      }
    }
  }
  return false;
}
//to check if the knight move about to be performed is valid
bool knightMoves(Square board[8][8],int selectedX,int selectedY,int x, int y) {
  if (board[y][x].base==EMPTY || board[y][x].base%2!=someCheck) {
    if ((y==selectedY-2 || y==selectedY+2) && ((x==selectedX+1) || x==selectedX-1) ) {
      return true;
    }
    else if ((x==selectedX+2 || x==selectedX-2) && ((y==selectedY-1)|| y==selectedY+1)) {
      return true;
    }
  }
  return false;
}
//to check if the bishop move about to be performed is valid
bool bishopMoves(Square board[8][8], int selectedX, int selectedY,int x, int y) {

  if (y!=selectedY && x!=selectedX) {
    int xGo= (x>selectedX)?1:-1;
    int yGo = (y>selectedY)?1:-1;
    for (int i=1;i<8;i++) {
      if (x==selectedX+i*xGo && y==selectedY+i*yGo) {
        return true;
      }
      else if (board[selectedY+i*yGo][selectedX+i*xGo].base!=EMPTY) {
        break;
      }
    } 

  }
  return false;
}
//to check if the queen move about to be performed is valid
bool queenMoves(Square board[8][8], int selectedX, int selectedY, int x,int y) {

  return (rookMoves(board, selectedX, selectedY,x,y) || bishopMoves(board, selectedX, selectedY,x,y)); 

}
//to check if the king move about to be performed is valid
bool kingMoves(Square board[8][8], int selectedX, int selectedY,int x, int y) {
  if (!(x==selectedX && y==selectedY)) {
    int xGo = (x > selectedX) ? 1 : (x < selectedX) ? -1 : 0;
    int yGo = (y > selectedY) ? 1 : (y < selectedY) ? -1 : 0;
    if (x==selectedX+1*xGo && y==selectedY+1*yGo) {
      return true;
    }
    else if (board[selectedY + 1* xGo][selectedX+1*xGo].base!=EMPTY) {
    }
  }
  return false;
}
