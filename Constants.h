#pragma once

const float PI = 3.14159265f;
const float PI_HALF = 1.57079632679f;
const float CENTER_X = 480.0f;
const float CENTER_Y = 272.0f;

const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 544;

const float SCALE = 4.0f; 

const int TILE_SIZE = 16;
const int DRAW_TILE_SIZE = (int)(TILE_SIZE * SCALE);

const int BOARD_PIXELS = 128; 
const int DRAW_BOARD_SIZE = (int)(BOARD_PIXELS * SCALE);

const int BOARD_OFFSET_X = (960 - DRAW_BOARD_SIZE) / 2; 
const int BOARD_OFFSET_Y = (544 - DRAW_BOARD_SIZE) / 2; 

enum PieceType { PAWN = 0, BISHOP = 1, KNIGHT = 2, ROOK = 3, KING = 4, QUEEN = 5, EMPTY = 6 };
enum PieceColor { WHITE = 0, BLACK = 1, NONE = 2 };
enum GameState {
    MAIN_MENU,
    HOW_TO_PLAY,
    PLAYING,
    WHITE_WINS,
    BLACK_WINS,
    DRAW
};

struct Piece{
    PieceType type;
    PieceColor color;
    bool hasMoved = false;
    Piece():type(EMPTY), color(NONE), hasMoved(false){};
    Piece(PieceType t, PieceColor c):type(t), color(c), hasMoved(false){}
  };

