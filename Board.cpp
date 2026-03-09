#include "Board.h"
#include <cmath>

Board::Board() {
    reset();
}

void Board::reset() {
    
    epX = -1;
    epY = -1;

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            grid[y][x] = {EMPTY, WHITE};
        }
    }
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            grid[y][x] = Piece(); 
        }
    }

    
    grid[0][0] = grid[0][7] = Piece(ROOK, BLACK);
    grid[0][1] = grid[0][6] = Piece(KNIGHT, BLACK);
    grid[0][2] = grid[0][5] = Piece(BISHOP, BLACK);
    grid[0][3] = Piece(QUEEN, BLACK);
    grid[0][4] = Piece(KING, BLACK);
    for (int x = 0; x < 8; x++) grid[1][x] = Piece(PAWN, BLACK);

    
    grid[7][0] = grid[7][7] = Piece(ROOK, WHITE);
    grid[7][1] = grid[7][6] = Piece(KNIGHT, WHITE);
    grid[7][2] = grid[7][5] = Piece(BISHOP, WHITE);
    grid[7][3] = Piece(QUEEN, WHITE);
    grid[7][4] = Piece(KING, WHITE);
    for (int x = 0; x < 8; x++) grid[6][x] = Piece(PAWN, WHITE);
}

Piece Board::getPiece(int x, int y) {
    
    if (x < 0 || x >= 8 || y < 0 || y >= 8) return Piece(); 
    return grid[y][x];
}

void Board::movePiece(int startX, int startY, int endX, int endY) {
    
    grid[endY][endX] = grid[startY][startX];
    grid[startY][startX] = Piece(); 
}
void Board::setPiece(int x, int y, Piece p) {
    if (x >= 0 && x < 8 && y >= 0 && y < 8) {
        grid[y][x] = p; 
    }
}
bool Board::isValidMove(int startX, int startY, int endX, int endY) {
    Piece movingPiece = getPiece(startX, startY);
    Piece targetPiece = getPiece(endX, endY);

    
    if (targetPiece.type != EMPTY && targetPiece.color == movingPiece.color) {
        return false;
    }

    
    int dx = endX - startX;
    int dy = endY - startY;

    
    switch (movingPiece.type) {
        case PAWN: {
            int dir = (movingPiece.color == WHITE) ? -1 : 1;
            int startRow = (movingPiece.color == WHITE) ? 6 : 1;

            
            if (dx == 0 && dy == dir && targetPiece.type == EMPTY) return true;

            
            if (dx == 0 && dy == 2 * dir && startY == startRow && targetPiece.type == EMPTY) {
                if (getPiece(startX, startY + dir).type == EMPTY) return true;
            }

            
            if ((dx == 1 || dx == -1) && dy == dir) {
                
                if (targetPiece.type != EMPTY) return true;

                
                if (endX == epX && endY == epY) return true;
            }
            

            return false;
        }

        case KNIGHT:
            
                
                    return (std::abs(dx) == 1 && std::abs(dy) == 2) || (std::abs(dx) == 2 && std::abs(dy) == 1);

        case BISHOP:
            
                if (std::abs(dx) == std::abs(dy)) {
                    return isPathClear(startX, startY, endX, endY);
                }
        return false;

        case ROOK:
            
                if (dx == 0 || dy == 0) {
                    return isPathClear(startX, startY, endX, endY);
                }
        return false;

        case QUEEN:
            
                if (dx == 0 || dy == 0 || std::abs(dx) == std::abs(dy)) {
                    return isPathClear(startX, startY, endX, endY);
                }
        return false;

        case KING: {
            
            if (std::abs(dx) <= 1 && std::abs(dy) <= 1) {
                return true;
            }

            
            if (!movingPiece.hasMoved && dy == 0 && std::abs(dx) == 2) {
                
                if (isInCheck(movingPiece.color)) return false;

                
                int rookX = (dx > 0) ? 7 : 0;
                Piece rook = getPiece(rookX, startY);

                
                if (rook.type == ROOK && !rook.hasMoved) {

                    
                    if (isPathClear(startX, startY, rookX, startY)) {

                        
                        
                        int stepX = (dx > 0) ? 1 : -1;
                        setPiece(startX + stepX, startY, movingPiece);
                        setPiece(startX, startY, {EMPTY, WHITE});

                        bool isStepSafe = !isInCheck(movingPiece.color);

                        
                        setPiece(startX, startY, movingPiece);
                        setPiece(startX + stepX, startY, {EMPTY, WHITE});

                        return isStepSafe; 
                    }
                }
            }
            return false;
        }

        default:
            return false;
    }
}
bool Board::isPathClear(int startX, int startY, int endX, int endY) {
    int dx = endX - startX;
    int dy = endY - startY;

    
    int stepX = (dx == 0) ? 0 : (dx > 0 ? 1 : -1);
    int stepY = (dy == 0) ? 0 : (dy > 0 ? 1 : -1);

    int currentX = startX + stepX;
    int currentY = startY + stepY;

    
    while (currentX != endX || currentY != endY) {
        if (getPiece(currentX, currentY).type != EMPTY) {
            return false; 
        }
        currentX += stepX;
        currentY += stepY;
    }

    return true; 
}
bool Board::isInCheck(int color) {
    int kingX = -1, kingY = -1;

    
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            Piece p = getPiece(x, y);
            if (p.type == KING && p.color == color) {
                kingX = x;
                kingY = y;
                break;
            }
        }
        if (kingX != -1) break; 
    }

    
    if (kingX == -1) return false;

    
    int enemyColor = (color == WHITE) ? BLACK : WHITE;

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            Piece p = getPiece(x, y);
            if (p.type != EMPTY && p.color == enemyColor) {
                
                if (isValidMove(x, y, kingX, kingY)) {
                    return true; 
                }
            }
        }
    }

    return false; 
}
bool Board::hasLegalMoves(int color) {
    for (int startY = 0; startY < 8; startY++) {
        for (int startX = 0; startX < 8; startX++) {
            Piece p = getPiece(startX, startY);

            
            if (p.type != EMPTY && p.color == color) {

                
                for (int endY = 0; endY < 8; endY++) {
                    for (int endX = 0; endX < 8; endX++) {

                        if (isValidMove(startX, startY, endX, endY)) {
                            
                            Piece targetPiece = getPiece(endX, endY);
                            setPiece(endX, endY, p);
                            setPiece(startX, startY, {EMPTY, WHITE});

                            
                            bool kingSafe = !isInCheck(color);

                            
                            setPiece(startX, startY, p);
                            setPiece(endX, endY, targetPiece);

                            
                            if (kingSafe) {
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }
    
    return false;
}