#pragma once

enum PieceType { PAWN = 0, BISHOP = 1, KNIGHT = 2, ROOK = 3, KING = 4, QUEEN = 5, EMPTY = 6 };
enum PieceColor { WHITE = 0, BLACK = 1, NONE = 2 };

struct Piece {
    PieceType type;
    PieceColor color;
    bool hasMoved = false;
    Piece() : type(EMPTY), color(NONE), hasMoved(false) {}
    Piece(PieceType t, PieceColor c) : type(t), color(c), hasMoved(false) {}
};