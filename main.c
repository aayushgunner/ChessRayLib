#include "raylib.h"
#include "actions.h"
#include "globals.h"
#include "render.h"
#include "bot.h"
int main(void)
{  

  InitWindow(useWidht, useHeight, "Chess");
  Texture2D *allTextures= textureLoad(imagePaths);
  Square board[8][8];
  SetTargetFPS(60);
  Vector2 ok;
  haamro.a=128;
  initializeBoard(board);
  initializePiece(board);
  initializeArray(availableMoves);
  bool interface=false;
  int botPlay=0;
  while (!WindowShouldClose())    
  // Detect window close button or ESC key
  {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    // Clear the background with white Color
    if (!interface) {
      renderStart();
      interface = buttonSelect();
    }

    else {

    if (botPlay==0) {
        selectMode();
        botPlay=buttonModeSelect();
      }
    else if (botPlay==2 || botPlay==1) {


      if (someCheck==1 && botPlay==2) {
        moveBot(board);
      }
        renderBoard(board,ok);
        highlightPiece(board);
        renderPieces(board,allTextures);
        promotionPiece(board);
        winnerLoad(board);

    }


    }
    // possibleMoves(board);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
