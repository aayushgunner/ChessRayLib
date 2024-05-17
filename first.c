#include "raylib.h"
#include <stdbool.h>
#include<stdlib.h>
#include <string.h>
#include <stdio.h>
const int useHeight = 800; //Window dimension height.
const int useWidht = 800;  //Window dimension width.
const int squaresize = 100; //size of the square of the boared  
bool isSelected = false;    //to check if the piece is selected or not  
int pawnNum=1;      //check which pawn 
int someCheck=0;     //to check which color piece
Color haamro = GREEN;  


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
int pieceEval[12]= {-1,1,-3,3,-3,3,-5,5,-9,9,0,0};       //assigning a value to each type of piece.
bool isWhite = true;
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




bool canMove(Square board[8][8],int selectedX,int selectedY, Color selectedColor , int x, int y);
void performMove(Square board[8][8],int selectedY,int selectedX,int x,int y) ;
bool pawnMoves(Square board[8][8],int selectedX, int selectedY,int x,int y );
bool rookMoves(Square board[8][8],int selectedX, int selectedY,int x, int y );
bool knightMoves(Square board[8][8],int selectedX, int selectedY,int x, int y);
bool bishopMoves(Square board[8][8],int selectedX, int selectedY,int x, int y  );
bool queenMoves(Square board[8][8],int selectedX, int selectedY, int x, int y );
bool kingMoves(Square board[8][8],int selectedX, int selectedY , int x, int y);
void addMoves (Move characs);                                                               //function declaration
GameState isgameFinished(Square board[8][8]);
GameState checkCondi (Square board[8][8]);
void possibleMoves(Square board[8][8]);
void initializeArray(Vector availableMoves);
void promotionPiece(Square board[8][8]);
int evalFunc(Square board[8][8]); 
int miniMaxbot(Square board[8][8], Move temp,int depth) ;
void undoMove(Square board[8][8], Move backGo);
Move *getCopy() ;


void moveBot(Square board[8][8]) {
  possibleMoves(board);
  Move *lyang = getCopy();
  size_t lyangsize=availableMoves.size;
  int bestEval=-100;
  Move bestMove=availableMoves.dynamicArra[0];         //to initialize the bot 
  for (int i=0;i<lyangsize;i++) {
    int eval=miniMaxbot(board, lyang[i],1);
    if (eval>bestEval) {
      bestEval=eval;
      bestMove=lyang[i];
    }
  }
  free(lyang);
  performMove(board, bestMove.khaanePieceY, bestMove.khaanePieceX, bestMove.marekoX, bestMove.marekoY);
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
  Move *hang= getCopy();
  size_t hangsize=availableMoves.size;
  for (int i=0;i<hangsize;i++) {
    eval=miniMaxbot(board,hang[i],depth+1) ;
    if (someCheck==1 && eval>besteval) {
      besteval=eval;
    } 
    else if (someCheck==0 && eval<besteval) {
      besteval=eval;
    }
  }
  free(hang);
  undoMove(board,temp);
  return besteval;
}

//evaluation of the board accoring to pieces
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
//to check validity of postion of pieces in the board
bool isboardInvalid(Square board[8][8]) {
  return ((checkCondi(board)==blackChecked && someCheck==1)|| (checkCondi(board)==whiteChecked && someCheck==0)|| checkCondi(board)==inValid);
}
void undoMove (Square board[8][8], Move backGo) {

  board[backGo.khaanePieceY][backGo.khaanePieceX].base=backGo.maarnePiece;
  board[backGo.marekoY][backGo.marekoX].base=backGo.marekoPiece;                   //undo the move
  someCheck= (someCheck==1)?0:1;
  // printf("%d \n", someCheck);
}
void possibleMoves(Square board[8][8]) {
  availableMoves.size=0;
  for (int i=0;i<8;i++) {
    for (int j=0;j<8;j++) {
      if (board[i][j].base!=EMPTY && board[i][j].base%2==someCheck) {
        for (int a=0;a<8;a++) {
          for (int b=0;b<8;b++) {
            if (canMove(board, j, i, WHITE, b, a) && (board[a][b].base==EMPTY || board[i][j].base%2!=board[a][b].base%2)  ) {
              Move temp;
              temp.marekoX=b;
              temp.marekoY=a;
              temp.khaanePieceX=j;
              temp.khaanePieceY=i;
              temp.marekoPiece=board[a][b].base;
              temp.maarnePiece=board[i][j].base;                                    //calcualte all the possible moves
              performMove(board, i, j, b, a);
              if (isboardInvalid(board)) {
                undoMove(board, temp);
                continue;
              }
              undoMove(board,temp);
              addMoves(temp);

            }
          }
        }
      }
    }
  }
  // printf("%lu size \n",availableMoves.size);
}


void renderStart() {
  DrawText("Lets start the Game", 300, 300, 20, BLACK);
  DrawText("Press Enter to begin the game", 400,400,20,BLACK);  //initial screen of the game
}

bool buttonSelect() {
  if (IsKeyPressed(KEY_ENTER)) {

    return true;

  }
  return false;
}

GameState checkCondi(Square board[8][8]) {                      //function to check whether a player is checked or not
  int blackKingX;
  int blackKingY;
  int whiteKingX;
  int whiteKingY;
  bool isBlack=false;
  bool isWhite=false;
  for (int i=0;i<8;i++) {
    for (int j=0;j<8;j++) {                              
      if (board[i][j].base==KINGB) {
        blackKingX=j;
        blackKingY=i;
      }
      if (board[i][j].base==KINGW) {
        whiteKingX=j;
        whiteKingY=i;
      }
    }
  }
  for (int i=0;i<8;i++) {
    for (int j=0;j<8;j++) {
      if (board[i][j].base!=EMPTY){
        if (board[i][j].base%2==0) {
          if (canMove(board, j, i, WHITE, whiteKingX , whiteKingY)) {
            // printf("WHite is checked");
            isWhite=true;
            // return whiteChecked;
          }
        }
        else  {
          if (canMove(board, j, i, WHITE, blackKingX, blackKingY)) {
            // printf("Black is checked");
            isBlack=true;
            // return blackChecked;
          }
        }
      }
    }
  }
  if (isWhite && isBlack) {
    return inValid;
  }

  else if (isWhite) {
    return whiteChecked;
  }

  else if (isBlack) {
    return blackChecked;
  }
  return playOn;
}

GameState isgameFinished(Square board[8][8]) {                         //check if the game is finished
  if (checkCondi(board)==whiteChecked) {
    possibleMoves(board);
    if (availableMoves.size==0) {
      return blackWins;
    }
  }
  else if (checkCondi(board)==blackChecked) {
    possibleMoves(board);
    if (availableMoves.size==0) {
      return whiteWins;
    }
  }
  return playOn;
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
void renderBoard(Square board[8][8], Vector2 ok) {
  for (int i=0;i<8;i++) {                                         
    for (int j = 0; j<8;j++) {
      DrawRectangle(board[i][j].x, board[i][j].y,squaresize , squaresize, board[i][j].color); //render the board
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
void renderPieces(Square board[8][8], Texture2D allTextures[]){

  for (int i=0;i<8;i++) {
    for (int j=0;j<8;j++) {
      if (board[i][j].base!=EMPTY) {
        DrawTexture(allTextures[board[i][j].base], 10+j*100 ,  10+i*100, GRAY );//render the pieces in their resepective positions
      } 
    }
  }
}
//check if a move is valid or not
bool canMove(Square board[8][8],int selectedX,int selectedY, Color selectedColor , int x, int y) {
  if (board[selectedY][selectedX].base==PAWNB || board[selectedY][selectedX].base==PAWNW) {
    return  pawnMoves(board,selectedX,selectedY, x, y);
  }
  if (board[selectedY][selectedX].base==ROOKW || board[selectedY][selectedX].base==ROOKB) {
    return rookMoves(board,selectedX,selectedY, x, y);
  }
                                                                                                    
  if (board[selectedY][selectedX].base==KNIGHTB || board[selectedY][selectedX].base==KNIGHTW) {
    return knightMoves(board,selectedX,selectedY, x, y);
  }
  if (board[selectedY][selectedX].base==BISHOPB || board[selectedY][selectedX].base==BISHOPW) {
    return bishopMoves(board,selectedX,selectedY,x, y);
  }
  if (board[selectedY][selectedX].base==QUEENB || board[selectedY][selectedX].base==QUEENW) {
    return queenMoves(board,selectedX,selectedY,x,y);
  }
  if (board[selectedY][selectedX].base==KINGB || board[selectedY][selectedX].base==KINGW) {
    return kingMoves(board,selectedX,selectedY,x,y);
  }
  return false;
}

//tp highlight the pieces
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

//to check if the pawnMove about to be performed is valid
bool pawnMoves(Square board[8][8],int selectedX, int selectedY, int x, int y  ) {
  if (board[selectedY][selectedX].base==PAWNB) {
    pawnNum=1;
  }
  else if (board[selectedY][selectedX].base==PAWNW) {
    pawnNum=-1;
  }
  if ((selectedY==1 || selectedY==6) && y==selectedY+pawnNum*2 && x==selectedX && (board[selectedY+pawnNum][x].base==EMPTY && board[selectedY+pawnNum*2][x].base==EMPTY)  ) {
    return true;
  }

  if (y==selectedY+pawnNum && x==selectedX && (board[y][x].base==EMPTY) || 
    (y==selectedY+pawnNum && (x==selectedX+pawnNum || x==selectedX-pawnNum) && board[y][x].base%2!=someCheck && board[y][x].base!=EMPTY)||
    (y==selectedY+pawnNum && ((x==selectedX && board[y][x].base==EMPTY)|| ((x==selectedX+pawnNum || x==selectedX-pawnNum)&&board[y][x].base%2!=someCheck && board[y][x].base!=EMPTY)) )
  ) {
    return true;
  }
  return false;
}
//to check if the rook move about to be performed is valid
bool rookMoves(Square board[8][8], int selectedX, int selectedY, int x, int y ) {
  if (x==selectedX ^ y==selectedY) {
    int xGo = (x > selectedX) ? 1 : (x < selectedX) ? -1 : 0;
    int yGo = (y > selectedY) ? 1 : (y < selectedY) ? -1 : 0;

    for (int i=1;i<8;i++) {
      if (x==selectedX + i*xGo && y==selectedY + i*yGo) {
        return true;
      }  
      else if (board[selectedY+i*yGo][selectedX+i*xGo].base!=EMPTY) {
        break;
      }
    }
  }
  return false;
}
//to check if the knight move about to be performed is valid
bool knightMoves(Square board[8][8],int selectedX,int selectedY,int x, int y) {
  if (board[y][x].base==EMPTY || board[y][x].base%2!=someCheck) {
    if ((y==selectedY-2 || y==selectedY+2) && ((x==selectedX+1) || x==selectedX-1) ) {
      return true;
    }
    else if ((x==selectedX+2 || x==selectedX-2) && ((y==selectedY-1)|| y==selectedY+1)) {
      return true;
    }
  }
  return false;
}
//to check if the bishop move about to be performed is valid
bool bishopMoves(Square board[8][8], int selectedX, int selectedY,int x, int y) {

  if (y!=selectedY && x!=selectedX) {
    int xGo= (x>selectedX)?1:-1;
    int yGo = (y>selectedY)?1:-1;
    for (int i=1;i<8;i++) {
      if (x==selectedX+i*xGo && y==selectedY+i*yGo) {
        return true;
      }
      else if (board[selectedY+i*yGo][selectedX+i*xGo].base!=EMPTY) {
        break;
      }
    } 

  }
  return false;
}
//to check if the queen move about to be performed is valid
bool queenMoves(Square board[8][8], int selectedX, int selectedY, int x,int y) {

  return (rookMoves(board, selectedX, selectedY,x,y) || bishopMoves(board, selectedX, selectedY,x,y)); 

}
//to check if the king move about to be performed is valid
bool kingMoves(Square board[8][8], int selectedX, int selectedY,int x, int y) {
  if (!(x==selectedX && y==selectedY)) {
    int xGo = (x > selectedX) ? 1 : (x < selectedX) ? -1 : 0;
    int yGo = (y > selectedY) ? 1 : (y < selectedY) ? -1 : 0;
    if (x==selectedX+1*xGo && y==selectedY+1*yGo) {
      return true;
    }
    else if (board[selectedY + 1* xGo][selectedX+1*xGo].base!=EMPTY) {
    }
  }
  return false;
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

  while (!WindowShouldClose())    // Detect window close button or ESC key
  {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    // Clear the background with white Color
    if (!interface) {

      renderStart();
      interface = buttonSelect();
    }

    else {

      // if (someCheck==1) {
      //
      //   printf("thikai cha");
      //
      //   moveBot(board);
      // }
      renderBoard(board,ok);
      highlightPiece(board);
      renderPieces(board,allTextures);
      promotionPiece(board);
      winnerLoad(board);

    }
    // possibleMoves(board);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
