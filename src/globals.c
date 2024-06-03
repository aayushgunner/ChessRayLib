#include "globals.h"

Move Played_move;
bool isSelected = false;    //to check if the piece is selected or not  
int pawnNum=1;      //check which pawn 
int someCheck=0;     //to check which color piece
Color haamro = GREEN;  
bool isWhite = true;
int pieceEval[12]= {-1,1,-3,3,-3,3,-5,5,-9,9,0,0};   //assigning a value to each type of piece.
const int useHeight=800 ; //Window dimension height.
const int useWidht=800;  //Window dimension width.
 const int squaresize=100;//size of the square of the boared 
Vector availableMoves;
char *imagePaths[] = {
  "./images/PAWNB.png",
  "./images/PAWNW.png",
  "./images/KNIGHTB.png",
  "./images/KNIGHTW.png",             //array to store the image paths.
  "./images/BISHOPB.png",
  "./images/BISHOPW.png",
  "./images/ROOKB.png",
  "./images/ROOKW.png",
  "./images/QUEENB.png",
  "./images/QUEENW.png",
  "./images/KINGB.png",
  "./images/KINGW.png"
}; 
