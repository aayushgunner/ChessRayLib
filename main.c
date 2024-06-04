#include <raylib.h>
#include<fcntl.h>
#include <stdbool.h>
#include "src/structEnum.h"
#include "src/actions.h"
#include "src/globals.h"
#include "src/render.h"
#include "src/bot.h"
#include "src/server.h"
#include "src/client.h"

int main(int argc , char * argv[])
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
  bool is_server = argv[1];
  bool move_performed = false;
  int sockfd = 0;
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
        if(botPlay ==1){
          // set is_server;
          if(is_server){
            sockfd = initialize_server(8898);
          }
          else{
            sockfd = initialize_client("127.0.0.1", 8898);
          }
        }

      }

      else if (botPlay==2 || botPlay==1) {
        if (someCheck==1 && botPlay==2) {
          moveBot(board);
        }
        else if(someCheck == 1 && !is_server){
          Move opponent_move = receive_serverMove(sockfd);
          performMove(board, opponent_move.khaanePieceY, opponent_move.khaanePieceX, opponent_move.marekoX, opponent_move.marekoY);
        }
        else if(someCheck == 0 && is_server){
          Move opponent_move = receive_clientMove(sockfd);
          performMove(board, opponent_move.khaanePieceY, opponent_move.khaanePieceX, opponent_move.marekoX, opponent_move.marekoY);
        }

        renderBoard(board,ok);
        move_performed = highlightPiece(board);

        if(is_server && move_performed){
          send_serverMove(sockfd, Played_move);
        }
        else if(!is_server && move_performed){
          send_clientMove(sockfd, Played_move);
        }

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
