#pragma once
#include <vita2d.h>
#include "Constants.h"
#include "Board.h"

class Renderer {
public:
    Renderer();
    ~Renderer();

    void drawBoard();
    void drawPieces(Board& board);
    void drawHighlight(int boardX, int boardY);
    void drawPromotionMenu(int currentTurnColor);
    void drawEndBanner(int currentTurnColor);
    void drawMainMenu();
    void drawHowToPlay();

private:
    vita2d_texture* boardTex;
    vita2d_texture* piecesTex;
    vita2d_texture* promoWhiteTex;
    vita2d_texture* promoBlackTex;
    vita2d_texture* EndBannerBlack;
    vita2d_texture* EndBannerWhite;
    vita2d_texture* EndBannerDraw;
    vita2d_texture* menuTex;
    vita2d_texture* howToPlayTex;
};