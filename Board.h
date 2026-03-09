#pragma once
#include "Constants.h"

class Board {
public:
    Board(); 
    void reset(); 
    Piece getPiece(int x, int y); 
    void movePiece(int startX, int startY, int endX, int endY); 
    void setPiece(int x, int y, Piece p);
    bool isValidMove(int startX, int startY, int endX, int endY);
    bool isPathClear(int startX, int startY, int endX, int endY);
    bool isInCheck(int color);
    bool hasLegalMoves(int color);
    int epX = -1;
    int epY = -1;


private:
    
    Piece grid[8][8];
};