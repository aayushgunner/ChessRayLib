#ifndef RENDER_H
#define RENDER_H
#include"headers.h"
#include "structEnum.h"
#include <stdbool.h>
void renderStart();
void selectMode();
int buttonModeSelect();
bool buttonSelect() ;
void renderPieces(Square board[8][8], Texture2D allTextures[]);
void renderBoard(Square board[8][8], Vector2 ok);
bool highlightPiece(Square board[8][8]);
void winnerLoad(Square board[8][8]); 
#endif // !RENDER_H
