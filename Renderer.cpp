#include "Board.h"
#include "renderer.h"

Renderer::Renderer() {
    boardTex = vita2d_load_PNG_file("app0:assets/board.png");
    piecesTex = vita2d_load_PNG_file("app0:assets/pieces2.png");
    promoWhiteTex = vita2d_load_PNG_file("app0:assets/promotion-white.png");
    promoBlackTex = vita2d_load_PNG_file("app0:assets/promotion-black.png");
    EndBannerBlack = vita2d_load_PNG_file("app0:assets/black-wins.png");
    EndBannerWhite = vita2d_load_PNG_file("app0:assets/white-wins.png");
    EndBannerDraw = vita2d_load_PNG_file("app0:assets/draw.png");
    menuTex = vita2d_load_PNG_file("app0:assets/main_menu.png");
    howToPlayTex = vita2d_load_PNG_file("app0:assets/how_to_play.png");

    
    if (boardTex) vita2d_texture_set_filters(boardTex, SCE_GXM_TEXTURE_FILTER_POINT, SCE_GXM_TEXTURE_FILTER_POINT);
    if (piecesTex) vita2d_texture_set_filters(piecesTex, SCE_GXM_TEXTURE_FILTER_POINT, SCE_GXM_TEXTURE_FILTER_POINT);
    if (menuTex) vita2d_texture_set_filters(menuTex, SCE_GXM_TEXTURE_FILTER_POINT, SCE_GXM_TEXTURE_FILTER_POINT);
    if (howToPlayTex) vita2d_texture_set_filters(howToPlayTex, SCE_GXM_TEXTURE_FILTER_POINT, SCE_GXM_TEXTURE_FILTER_POINT);
}

Renderer::~Renderer() {
    if (boardTex) vita2d_free_texture(boardTex);
    if (piecesTex) vita2d_free_texture(piecesTex);
    if (promoWhiteTex) vita2d_free_texture(promoWhiteTex);
    if (promoBlackTex) vita2d_free_texture(promoBlackTex);
    if (EndBannerWhite) vita2d_free_texture(EndBannerWhite);
    if (EndBannerBlack) vita2d_free_texture(EndBannerBlack);
    if (EndBannerDraw) vita2d_free_texture(EndBannerDraw);
    if(menuTex) vita2d_free_texture(menuTex);
    if(howToPlayTex) vita2d_free_texture(howToPlayTex);
}

void Renderer::drawBoard() {
    if (boardTex) {
        
        vita2d_draw_texture_scale(boardTex, BOARD_OFFSET_X, BOARD_OFFSET_Y, SCALE, SCALE);
    }
}

void Renderer::drawPieces(Board& board) {
    if (!piecesTex) return;

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            Piece p = board.getPiece(x, y);

            if (p.type != EMPTY) {
                
                float srcX = (float)p.type * TILE_SIZE;
                float srcY = (float)p.color * TILE_SIZE;

                
                
                
                float screenX = BOARD_OFFSET_X + (x * DRAW_TILE_SIZE) + (DRAW_TILE_SIZE / 2.0f);
                float screenY = BOARD_OFFSET_Y + (y * DRAW_TILE_SIZE) + (DRAW_TILE_SIZE / 2.0f);

                
                
                float angle = (p.color == BLACK) ? PI : 0.0f;

                vita2d_draw_texture_part_scale_rotate(piecesTex,
                                                      screenX, screenY,
                                                      srcX, srcY,
                                                      TILE_SIZE, TILE_SIZE,
                                                      SCALE, SCALE,
                                                      angle);
            }
        }
    }
}

void Renderer::drawHighlight(int boardX, int boardY) {
    if (boardX < 0 || boardX > 7 || boardY < 0 || boardY > 7) return;

    
    int screenX = BOARD_OFFSET_X + (boardX * DRAW_TILE_SIZE);
    int screenY = BOARD_OFFSET_Y + (boardY * DRAW_TILE_SIZE);

    
    vita2d_draw_rectangle(screenX, screenY, DRAW_TILE_SIZE, DRAW_TILE_SIZE, RGBA8(255, 255, 0, 128));
}

void Renderer::drawPromotionMenu(int currentTurnColor) {
    
    vita2d_texture* tex = (currentTurnColor == WHITE) ? promoWhiteTex : promoBlackTex;

    if (!tex) return;

    
    vita2d_draw_rectangle(0, 0, 960, 544, RGBA8(0, 0, 0, 150));

    
    float screenWidth = 64.0f * SCALE;
    float screenHeight = 17.0f * SCALE;

    
    float startX = CENTER_X - (screenWidth / 2.0f);
    float startY = CENTER_Y - (screenHeight / 2.0f);

    if (currentTurnColor == BLACK) {
        
        float rotatedX = startX + screenWidth;
        float rotatedY = startY + screenHeight;

        
        vita2d_draw_texture_part_scale_rotate(tex,
                                              CENTER_X, CENTER_Y, 
                                              0, 0,               
                                              64, 17,             
                                              SCALE, SCALE,       
                                              PI);                
    } else {
        
        vita2d_draw_texture_scale(tex, startX, startY, SCALE, SCALE);
    }
}
void Renderer::drawEndBanner(int winnerCode) {
    vita2d_texture* tex = nullptr;

    if (winnerCode == WHITE) {
        tex = EndBannerWhite;
    } else if (winnerCode == BLACK) {
        tex = EndBannerBlack;
    } else {
        tex = EndBannerDraw; 
    }
    if (!tex) return;

    vita2d_draw_rectangle(0, 0, 960, 544, RGBA8(0, 0, 0, 150));

    float screenWidth = 128.0f * SCALE;
    float screenHeight = 32.0f * SCALE;

    float startX = CENTER_X - (screenWidth / 2.0f);
    float startY = CENTER_Y - (screenHeight / 2.0f);

    if (winnerCode == BLACK) {
        vita2d_draw_texture_part_scale_rotate(tex,
                                                  CENTER_X, CENTER_Y, 
                                                  0, 0,               
                                                  128, 32,             
                                                  SCALE, SCALE,       
                                                  PI);                
    } else if (winnerCode == WHITE) {
        
        vita2d_draw_texture_scale(tex, startX, startY, SCALE, SCALE);
    } else {
        vita2d_draw_texture_scale(tex, startX, startY, SCALE, SCALE);
    }
}
void Renderer::drawMainMenu() {
    if (menuTex) {
        
        float w = vita2d_texture_get_width(menuTex) * SCALE;
        float h = vita2d_texture_get_height(menuTex) * SCALE;
        vita2d_draw_texture_scale(menuTex, CENTER_X - w/2.0f, CENTER_Y - h/2.0f, SCALE, SCALE);
    }
}

void Renderer::drawHowToPlay() {
    if (howToPlayTex) {
        float w = vita2d_texture_get_width(howToPlayTex) * SCALE;
        float h = vita2d_texture_get_height(howToPlayTex) * SCALE;
        vita2d_draw_texture_scale(howToPlayTex, CENTER_X - w/2.0f, CENTER_Y - h/2.0f, SCALE, SCALE);
    }
}