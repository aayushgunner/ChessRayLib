#include "render.h"
#include "globals.h"
#include "actions.h"
#include "possible.h"
#include "moves.h"
#include <raylib.h>
void renderStart() {
  DrawText("Lets start the Game", 300, 300, 20, BLACK);
  DrawText("Press Enter to begin the game", 400,400,20,BLACK);  //initial screen of the game
}

void selectMode() {
  DrawText("Press A for 2 player Mode", 300,300,20,BLACK);
  DrawText("Press B to play against bot", 400,400,20,BLACK);
}

int buttonModeSelect() {
  
  if (IsKeyPressed(KEY_A)) {
    return 1;
  }
  if (IsKeyPressed(KEY_B)) {
  return 2;
}
return 0;
}
 bool buttonSelect() {
  return IsKeyPressed(KEY_ENTER);
}

void renderBoard(Square board[8][8], Vector2 ok) {
  for (int i=0;i<8;i++) {                                         
    for (int j = 0; j<8;j++) {
      DrawRectangle(board[i][j].x, board[i][j].y,squaresize , squaresize, board[i][j].color); //render the board
    }
  }
}
void renderPieces(Square board[8][8], Texture2D allTextures[]){

  for (int i=0;i<8;i++) {
    for (int j=0;j<8;j++) {
      if (board[i][j].base!=EMPTY) {
        DrawTexture(allTextures[board[i][j].base], 10+j*100 ,  10+i*100, GRAY );//render the pieces in their resepective positions
      } 
    }
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
    DrawRectangle(board[y][x].x, board[y][x].y,squaresize , squaresize, BLUE);//highlights the piece when mouse is hovered
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
    DrawRectangle(board[selectedY][selectedX].x, board[selectedY][selectedX].y, squaresize,squaresize, RED);//higlights the piece red when it is clicked
    for (int a=0;a<8;a++) {
      for (int b=0;b<8;b++) {
        if (canMove(board, selectedX, selectedY, WHITE, b, a) && (board[a][b].base==EMPTY || board[a][b].base%2!=someCheck)) {
          Move temp;
          temp.marekoX=b;
          temp.marekoY=a;
          temp.khaanePieceX=selectedX;
          temp.khaanePieceY=selectedY;
          temp.marekoPiece=board[a][b].base;
          temp.maarnePiece=board[selectedY][selectedX].base;
          performMove(board, selectedY, selectedX, b, a);
          if (!isboardInvalid(board)) {
            DrawRectangle(board[a][b].x, board[a][b].y, squaresize,squaresize, haamro); //the possible moves are highlighted
          }
          undoMove(board, temp);
        }}}
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && canMove(board,selectedX,selectedY,selectedColor, x,y)) {
      Move temp;
      temp.marekoX=x;
      temp.marekoY=y;
      temp.khaanePieceX=selectedX;
      temp.khaanePieceY=selectedY;
      temp.marekoPiece=board[y][x].base;
      temp.maarnePiece=board[selectedY][selectedX].base;
      isSelected=false;
      performMove(board, selectedY, selectedX, x , y);
      if (isboardInvalid(board)) {
        undoMove(board, temp);
      } 
    }
  }
}
void winnerLoad(Square board[8][8]) {
  if (isgameFinished(board)==playOn) {

    return;
  }

    DrawRectangle(250, 250, 400, 400, Fade(BLACK, 0.4f));
  if (isgameFinished(board) ==whiteWins) {
    DrawText("White wins", 300, 300, 60, BLACK);
  }
  else if (isgameFinished(board)==blackWins) {
    DrawText("Black wins", 300, 300, 60, BLACK);
  }
  DrawText("Press Enter to start new game", 100,500,30,BLACK);
    
  if (IsKeyPressed(KEY_ENTER)) {
        initializeBoard(board);
        initializePiece(board);
        initializeArray(availableMoves); 
  }
    

}

