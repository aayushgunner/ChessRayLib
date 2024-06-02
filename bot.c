#include "bot.h"
#include "globals.h"
#include "possible.h"
#include "actions.h"
int evalFunc(Square board[8][8]) {
  int eval=0;
  for (int i=0;i<8;i++) {
    for (int j=0;j<8;j++) {

      if (board[i][j].base!=EMPTY)  {
        eval+=pieceEval[board[i][j].base];
      }
    }
  }
  return eval;
}
int miniMaxbot(Square board[8][8], Move temp,int depth) {
  performMove(board, temp.khaanePieceY, temp.khaanePieceX, temp.marekoX, temp.marekoY);
  int besteval= (someCheck==1)?-100:100; 
  GameState check=isgameFinished(board);
  if (check==whiteWins || check==blackWins) {
    undoMove(board, temp);
    return besteval;
  }
  else if (check==draw) {
    undoMove(board,temp);
    return 0;
  }    
  //minimax algortithm   
  //depth set here
  if (depth>2) {                                                                   
    besteval=evalFunc(board);
    undoMove(board,temp);
    
    return besteval;
  }
  int eval;
  possibleMoves(board);
  Move *temp1= getCopy();
  size_t temp1SizeSize=availableMoves.size;
  for (int i=0;i<temp1SizeSize;i++) {
    eval=miniMaxbot(board,temp1[i],depth+1) ;
    if (someCheck==1 && eval>besteval) {
      besteval=eval;
    } 
    else if (someCheck==0 && eval<besteval) {
      besteval=eval;
    }
  }
  free(temp1);
  undoMove(board,temp);
  return besteval;
}

void moveBot(Square board[8][8]) {
  possibleMoves(board);
  Move *temp = getCopy();
  size_t tempSize=availableMoves.size;
  int bestEval=-100;
  Move bestMove=availableMoves.dynamicArra[0];         //to initialize the bot 
  for (int i=0;i<tempSize;i++) {
    int eval=miniMaxbot(board, temp[i],1);
    if (eval>bestEval) {
      bestEval=eval;
      bestMove=temp[i];
    }
  }
  free(temp);
  performMove(board, bestMove.khaanePieceY, bestMove.khaanePieceX, bestMove.marekoX, bestMove.marekoY);
}

