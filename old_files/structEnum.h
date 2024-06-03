#ifndef ITEM_H
#define ITEM_H
#include "headers.h"
typedef enum {
  PAWNB,
  PAWNW,
  KNIGHTB,
  KNIGHTW,
  BISHOPB,
  BISHOPW,                 //Pieces
  ROOKB,
  ROOKW,
  QUEENB,
  QUEENW,
  KINGB,
  KINGW,
  EMPTY
} PieceType;

typedef struct {
  int khaanePieceX;
  int khaanePieceY;
  int marekoX;
  int marekoY;                       //structure to save the positions of eating and eaten piece.
  PieceType marekoPiece;
  PieceType maarnePiece;
} Move;


typedef struct {
  Move *dynamicArra;
  size_t size ;                       //simple duplicatio of vector type but not exactly
} Vector;

typedef enum {
  blackChecked,
  whiteChecked,
  blackWins,
  whiteWins,
  draw,
  playOn,
  inValid                                                 // conditions for the game to either stop or keep running
} GameState;

typedef struct {
  Texture2D texture;                                    
} Piece;
typedef struct {
  int x ;
  int y;
  Color color;
  PieceType base;                                               //structure to give characteristics to each position in the board.
  bool isBlack;
  bool isMoved;
} Square;


#endif
