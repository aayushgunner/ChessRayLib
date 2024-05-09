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

void pawnMoves(Square board[8][8],int selectedX, int selectedY, Color selectedColor);

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
    board[1][i].isMoved=false;
    board[6][i].base = PAWNW;
    board[6][i].isMoved=true;
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
  // printf("Lets go motherfuckers %d %d \n", selectedX, selectedY );
  if (board[selectedY][selectedX].base==PAWNB || board[selectedY][selectedX].base==PAWNW) {
    pawnMoves(board,selectedX,selectedY,selectedColor);
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


void pawnMoves(Square board[8][8],int selectedX, int selectedY, Color selectedColor) {

int x,y;
      // printf("Lets goo retards %d %d \n", selectedX, selectedY);
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
        board[y][x].base = board[selectedY][selectedX].base;
        board[selectedY][selectedX].base=EMPTY;
        isSelected= false;
        board[y][x].isMoved=true;
        if (someCheck==0) {
        someCheck=1;
      }
        else 
        someCheck=0;
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
  // Close window and OpenGL context
  CloseWindow();
  return 0;
}

