#include "raylib.h"
#include <stdbool.h>
#include<stdlib.h>
#include <stdio.h>
const int useHeight = 800;
const int useWidht = 800;
const int squaresize = 100;
bool isSelected = false;
int pawnNum=1;
int someCheck=0;

char *imagePaths[] = {
  "./images/PAWNB.png",
  "./images/PAWNW.png",
  "./images/KNIGHTB.png",
  "./images/KNIGHTW.png",
  "./images/BISHOPB.png",
  "./images/BISHOPW.png",
  "./images/ROOKB.png",
  "./images/ROOKW.png",
  "./images/QUEENB.png",
  "./images/QUEENW.png",
  "./images/KINGB.png",
  "./images/KINGW.png"
}; 
bool isWhite = true;
typedef enum {
  PAWNB,
  PAWNW,
  KNIGHTB,
  KNIGHTW,
  BISHOPB,
  BISHOPW,
  ROOKB,
  ROOKW,
  QUEENB,
  QUEENW,
  KINGB,
  KINGW,
  EMPTY
} PieceType;



typedef struct {
  Texture2D texture;
} Piece;

typedef struct {
  int x ;
  int y;
  Color color;
  PieceType base;
  bool isBlack;
  bool isMoved;
} Square;
void pawnMoves(Square board[8][8],int selectedX, int selectedY );
void rookMoves(Square board[8][8],int selectedX, int selectedY );
void knightMoves(Square board[8][8],int selectedX, int selectedY );
void bishopMoves(Square board[8][8],int selectedX, int selectedY );
void queenMoves(Square board[8][8],int selectedX, int selectedY );
void kingMoves(Square board[8][8],int selectedX, int selectedY );
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
        }
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
  Texture2D *textures = malloc(12 * sizeof(Texture2D));
  for (int i=0;i<12;i++) {
    textures[i] = LoadTexture(imagePaths[i]);
  }
  return textures;
}
void renderBoard(Square board[8][8], Vector2 ok) {
  for (int i=0;i<8;i++) {
    for (int j = 0; j<8;j++) {
      DrawRectangle(board[i][j].x, board[i][j].y,squaresize , squaresize, board[i][j].color);
    }
  }
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
  board[0][2].base = BISHOPB;
  board[0][5].base= BISHOPB;
  board[7][2].base = BISHOPW;
  board[7][5].base = BISHOPW;
  board[0][3].base = QUEENB;
  board[7][3].base = QUEENW;
  board[0][4].base = KINGB;
  board[7][4].base = KINGW;
}
void renderPieces(Square board[8][8], Texture2D allTextures[]){

  for (int i=0;i<8;i++) {
    for (int j=0;j<8;j++) {
      if (board[i][j].base!=EMPTY) {
        DrawTexture(allTextures[board[i][j].base], 10+j*100 ,  10+i*100, GRAY );
      } 
    }
  }
}

void doMoves(Square board[8][8],int selectedX,int selectedY, Color selectedColor ) {
  if (board[selectedY][selectedX].base==PAWNB || board[selectedY][selectedX].base==PAWNW) {
    pawnMoves(board,selectedX,selectedY);
  }
  if (board[selectedY][selectedX].base==ROOKW || board[selectedY][selectedX].base==ROOKB) {
    rookMoves(board,selectedX,selectedY);
  }

  if (board[selectedY][selectedX].base==KNIGHTB || board[selectedY][selectedX].base==KNIGHTW) {
    knightMoves(board,selectedX,selectedY);
  }
  if (board[selectedY][selectedX].base==BISHOPB || board[selectedY][selectedX].base==BISHOPW) {
    bishopMoves(board,selectedX,selectedY);
  }
  if (board[selectedY][selectedX].base==QUEENB || board[selectedY][selectedX].base==QUEENW) {
    queenMoves(board,selectedX,selectedY);
  }
  if (board[selectedY][selectedX].base==KINGB || board[selectedY][selectedX].base==KINGW) {
    kingMoves(board,selectedX,selectedY);
  }


}

void highlightPiece(Square board[8][8]) {
  static int selectedX = -1;
  static int selectedY = -1;
  static Color selectedColor = RED ;
  int x,y;
  x = GetMouseX()/100;
  y = GetMouseY()/100;
  if (board[y][x].base!=EMPTY && board[y][x].base%2==someCheck) { 
    DrawRectangle(board[y][x].x, board[y][x].y,squaresize , squaresize, BLUE);
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      if (!isSelected || (selectedX!=x || selectedY!=y) ) 
      {     
        selectedX = x;
        selectedY = y;
        isSelected = true;
      }
      else  {
        isSelected=false;
      }
    }
  }
  if (isSelected ) {
    DrawRectangle(board[selectedY][selectedX].x, board[selectedY][selectedX].y, squaresize,squaresize, RED);
    doMoves(board,selectedX,selectedY,selectedColor);
  }
}
void performMove(Square board[8][8],int selectedY,int selectedX,int x,int y) {

  board[y][x].base = board[selectedY][selectedX].base;
  board[selectedY][selectedX].base=EMPTY;
  isSelected= false;
  board[y][x].isMoved=true;
  if (someCheck==0) {
    someCheck=1;
  }
  else { 
    someCheck=0;
  }   
}
void pawnMoves(Square board[8][8],int selectedX, int selectedY  ) {
  int x,y;
  if (board[selectedY][selectedX].base==PAWNB) {
    pawnNum=1;
  }
  else if (board[selectedY][selectedX].base==PAWNW) {
    pawnNum=-1;
  }
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) ) {
    x=GetMouseX()/100;
    y=GetMouseY()/100;
    printf("Lets go g to the g %d %d \n", x,y);
    if (y==selectedY+pawnNum && x==selectedX && (board[y][x].base==EMPTY) || 
      (y==selectedY+pawnNum && (x==selectedX+pawnNum || x==selectedX-pawnNum) && board[y][x].base%2!=someCheck && board[y][x].base!=EMPTY)||
      (y==selectedY+pawnNum && ((x==selectedX && board[y][x].base==EMPTY)|| ((x==selectedX+pawnNum || x==selectedX-pawnNum)&&board[y][x].base%2!=someCheck && board[y][x].base!=EMPTY)) )
    ) {
      performMove(board,selectedY, selectedX, x, y);
    }
  }
}


void rookMoves(Square board[8][8], int selectedX, int selectedY ) {
  int x,y;
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    x=GetMouseX()/100;
    y=GetMouseY()/100;
    if (x==selectedX ^ y==selectedY) {
      int xGo = (x > selectedX) ? 1 : (x < selectedX) ? -1 : 0;
      int yGo = (y > selectedY) ? 1 : (y < selectedY) ? -1 : 0;

      for (int i=1;i<8;i++) {
        if (x==selectedX + i*xGo && y==selectedY + i*yGo) {
          performMove(board, selectedY, selectedX, x, y);
        }  
        else if (board[selectedY+i*yGo][selectedX+i*xGo].base!=EMPTY) {
          break;
        }

      }

      // if (y==selectedY) {
      //   if (x>selectedX) {
      //     for (int i=selectedX+1;i<=x;i++) {
      //       if (i==x) {
      //         performMove(board, selectedY, selectedX, x, y);
      //       }
      //       else if (board[y][i].base!=EMPTY ) {
      //         break;
      //       } 
      //     }
      //
      //   else if (selectedX>x) {
      //     for (int i=selectedX-1;i>=x;i--) {
      //       if (i==x) {
      //         performMove(board, selectedY, selectedX, x, y);
      //       }
      //       else if (board[y][i].base!=EMPTY) {
      //         break;
      //       }
      //     }
      //   }
      // }
      // if (x==selectedX) {
      //   if (y>selectedY) {
      //     for (int i=selectedY+1;i<=y;i++) {
      //       if (i==y) {
      //         performMove(board, selectedY, selectedX, x, y);
      //       }
      //       else if (board[i][x].base!=EMPTY) {
      //         break;
      //       }  
      //     }
      //   }
      //   else if (y<selectedY) {int deltaX = (x > selectedX) ? 1 : (x < selectedX) ? -1 : 0;
      //     for (int i=selectedY-1;i>=y;i--) {
      //       if (i==y) {
      //         performMove(board, selectedY, selectedX, x, y);
      //       }
      //       else if (board[i][x].base!=EMPTY) {
      //         break;
      //       }
      //     }
      //   }
    }
  }
}

void knightMoves(Square board[8][8],int selectedX,int selectedY) {
  int x,y;
  if   (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
    x=GetMouseX()/100;
    y=GetMouseY()/100;
    if (board[y][x].base==EMPTY || board[y][x].base%2!=someCheck) {
      if ((y==selectedY-2 || y==selectedY+2) && ((x==selectedX+1) || x==selectedX-1) ) {
        performMove(board, selectedY, selectedX, x, y);
      }
      else if ((x==selectedX+2 || x==selectedX-2) && ((y==selectedY-1)|| y==selectedY+1)) {
        performMove(board,selectedY,selectedX,x,y);
      }
    }
  }
}

void bishopMoves(Square board[8][8], int selectedX, int selectedY) {
  int x,y;
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    x=GetMouseX()/100;
    y=GetMouseY()/100;


    if (y!=selectedY && x!=selectedX) {
      int xGo= (x>selectedX)?1:-1;
      int yGo = (y>selectedY)?1:-1;
      for (int i=1;i<8;i++) {
        if (x==selectedX+i*xGo && y==selectedY+i*yGo) {
          performMove(board, selectedY, selectedX, x, y);
        }
        else if (board[selectedY+i*yGo][selectedX+i*xGo].base!=EMPTY) {
          break;
        }
      }
    } 

    // if (y>selectedY) {
    //   if (x>selectedX) {
    //    for (int i=1;i<8;i++) {
    //     if (x==selectedX+i && y==selectedY+i) {
    //         performMove(board, selectedY, selectedX, x, y);
    //       }
    //     else if (board[selectedY+i][selectedX+i].base!=EMPTY) {
    //       break;
    //     }
    //     }
    //   }
    //   else if (x<selectedX) {
    //     for (int i=1;i<8;i++) {
    //       if (x==selectedX-i && y==selectedY+i) {
    //         performMove(board, selectedY, selectedX, x, y);
    //       }
    //       else if (board[selectedY+i][selectedX-i].base!=EMPTY) {
    //         break;
    //       }
    //     }
    //     
    //   }
    // }
    //
    // else if (y<selectedY) {
    //   if (x>selectedX) {
    //     for (int i=1;i<8;i++) {
    //       if (x==selectedX+i && y==selectedY-i) {
    //         performMove(board, selectedY, selectedX, x, y);
    //       }
    //       else if (board[selectedY-i][selectedX+i].base!=EMPTY) {
    //         break;
    //       }
    //     }
    //   }
    //
    //   else if (x<selectedX) {
    //     for (int i=1;i<8;i++) {
    //       if (x==selectedX-i && y==selectedY-i) {
    //         performMove(board, selectedY, selectedX, x, y);
    //       }
    //       if (board[selectedY-i][selectedX-i].base!=EMPTY) {
    //         break;
    //       }
    //     }
    //   }
  }
}

void queenMoves(Square board[8][8], int selectedX, int selectedY) {

  rookMoves(board, selectedX, selectedY);
  bishopMoves(board, selectedX, selectedY);
}

void kingMoves(Square board[8][8], int selectedX, int selectedY) {
  int x,y;
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    x=GetMouseX()/100;
    y= GetMouseY()/100;
    if (!(x==selectedX && y==selectedY)) {
      int xGo = (x > selectedX) ? 1 : (x < selectedX) ? -1 : 0;
      int yGo = (y > selectedY) ? 1 : (y < selectedY) ? -1 : 0;
      if (x==selectedX+1*xGo && y==selectedY+1*yGo) {
        performMove(board, selectedY, selectedX, x, y);
      }

      else if (board[selectedY + 1* xGo][selectedX+1*xGo].base!=EMPTY) {
      }

    }

  }
}

int main(void)
{   
  InitWindow(useWidht, useHeight, "Chess");
  Texture2D *allTextures= textureLoad(imagePaths);
  Square board[8][8];
  SetTargetFPS(60);
  Vector2 ok;
  initializeBoard(board);
  initializePiece(board);
  while (!WindowShouldClose())    // Detect window close button or ESC key
  {
    BeginDrawing();
    ClearBackground(RAYWHITE);    // Clear the background with white Color
    renderBoard(board,ok);
    highlightPiece(board);
    renderPieces(board,allTextures);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}

