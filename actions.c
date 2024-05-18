#include "actions.h"
void undoMove (Square board[8][8], Move backGo) {

  board[backGo.khaanePieceY][backGo.khaanePieceX].base=backGo.maarnePiece;
  board[backGo.marekoY][backGo.marekoX].base=backGo.marekoPiece;                   //undo the move
  someCheck= (someCheck==1)?0:1;
  // printf("%d \n", someCheck);
}
void initializeArray (Vector availableMoves) {
  availableMoves.dynamicArra=NULL;                        //initialiae the array of storing the availableMoves
  availableMoves.size=0;
}
void addMoves (Move characs ) {
  availableMoves.dynamicArra = (Move *) realloc(availableMoves.dynamicArra,(availableMoves.size+1) * sizeof(Move));
  availableMoves.dynamicArra[availableMoves.size] = characs;   //too add possible moves
  availableMoves.size++;

}
void initializeBoard(Square board[8][8]){
  for (int x=0;x<8;x++) {
    for (int y=0;y<8;y++) {
      board[x][y].x = y*squaresize;
      board[x][y].y = x*squaresize;
      if (isWhite) {
        if ((y%2)==1) {
          board[x][y].color = BROWN;
        }
        else {
          board[x][y].color = RAYWHITE;
        }                                              //initialize board
      }
      else {
        if ((y%2)==1) {
          board[x][y].color = RAYWHITE;
        }
        else {
          board[x][y].color = BROWN;
        }
      }
    }
    isWhite = !isWhite;
  }
}
Texture2D * textureLoad(char *imagePaths[]) {
  Texture2D *textures = malloc(12 * sizeof(Texture2D));                 //load textures of the pieces 
  for (int i=0;i<12;i++) {
    textures[i] = LoadTexture(imagePaths[i]);
  }
  return textures;
}
void initializePiece(Square board[8][8]){
  for (int i=0;i<8;++i) {
    board[1][i].base = PAWNB;
    board[6][i].base = PAWNW;
  }
  for (int i=2;i<6;i++) {
    for (int j=0;j<8;j++) {
      board[i][j].base= EMPTY;
    }
  }
  board[0][0].base = ROOKB;
  board[0][7].base= ROOKB;
  board[7][0].base = ROOKW;
  board[7][7].base = ROOKW;
  board[0][1].base = KNIGHTB;
  board[0][6].base= KNIGHTB;
  board[7][1].base = KNIGHTW;
  board[7][6].base = KNIGHTW;
  board[0][2].base = BISHOPB;                                      //initialize the pieces
  board[0][5].base= BISHOPB;
  board[7][2].base = BISHOPW;
  board[7][5].base = BISHOPW;
  board[0][3].base = QUEENB;
  board[7][3].base = QUEENW;
  board[0][4].base = KINGB;
  board[7][4].base = KINGW;
  // board[7][5].base = QUEENB;
}
void promotionPiece(Square board[8][8]) {
  for (int i=0;i<7;i++) {
    if (board[0][i].base==PAWNW) {                        
      board[0][i].base=QUEENW;                  
    }                                                       //promotes pawn to queen if it reaches its respective end of the board.
    
    if (board[7][i].base==PAWNB) {
      board[7][i].base=QUEENB;
    }
  }


}


Move *getCopy() {
  Move *copy = (Move *) malloc(availableMoves.size* sizeof(Move));
  memcpy(copy, availableMoves.dynamicArra, availableMoves.size * sizeof(Move)); //for copying the availableMoves
  return copy;
}
void performMove(Square board[8][8],int selectedY,int selectedX,int x,int y) {
  board[y][x].base=board[selectedY][selectedX].base;
  board[selectedY][selectedX].base=EMPTY;                                       //peforms the respective move
  board[y][x].isMoved=true;
  if (someCheck==0) {
    someCheck=1;
  }
  else { 
    someCheck=0;
  }  
}
