#include <vita2d.h>
#include <psp2/kernel/processmgr.h>
#include <cmath>
#include "board.h"
#include "renderer.h"
#include "input.h"
#include <vector>

int main() {
    vita2d_init();
    vita2d_set_clear_color(RGBA8(40, 40, 40, 255));

    Board chessBoard;
    Renderer renderer;
    Input input; 

    int selectedX = -1; 
    int selectedY = -1;
    bool isPromoting = false;
    int promoX = -1; 
    int promoY = -1;

    int currentTurn = WHITE;
    GameState state = MAIN_MENU;

    std::vector<Board> boardHistory;
    std::vector<int> turnHistory;


    while (true) {
      
        if (input.isStartPressed()) {
            chessBoard.reset();
            currentTurn = WHITE;
            state = PLAYING;
            selectedX = -1;
            selectedY = -1;
            isPromoting = false;
        }
        if (input.isSelectPressed()) {
            
            if (!boardHistory.empty()) {
                
                chessBoard = boardHistory.back();
                currentTurn = turnHistory.back();

                
                boardHistory.pop_back();
                turnHistory.pop_back();

                
                state = PLAYING;
                selectedX = -1;
                selectedY = -1;
                isPromoting = false;
            }
        }

        bool checkGameEnd = false;

        
        if (state == MAIN_MENU) {
            int touchX, touchY;
            if (input.getRawTouch(touchX, touchY)) {
                float btnW = 966.0f; 
                float btnH = 544.0f;  

                
                float btnX = CENTER_X - (btnW / 2.0f);
                float btnY = CENTER_Y - (btnH / 2.0f);

                
                if (touchX >= btnX && touchX <= btnX + btnW &&
                    touchY >= btnY && touchY <= btnY + btnH) {

                    
                    state = HOW_TO_PLAY;
                    }
            }
        }
        else if (state == HOW_TO_PLAY) {
            int touchX, touchY;
            if (input.getRawTouch(touchX, touchY)) {
                
                chessBoard.reset(); 
                boardHistory.clear();
                turnHistory.clear();
                currentTurn = WHITE;
                state = PLAYING; 
            }
        }
        else if (state == WHITE_WINS || state == BLACK_WINS || state == DRAW) {
            
        }
        else if (isPromoting) {
            
            int touchX, touchY;
            if (input.getRawTouch(touchX, touchY)) {

                float menuWidth = 64.0f * SCALE;
                float menuHeight = 17.0f * SCALE;
                float menuStartX = CENTER_X - (menuWidth / 2.0f);
                float menuStartY = CENTER_Y - (menuHeight / 2.0f);

                if (touchX >= menuStartX && touchX <= menuStartX + menuWidth &&
                    touchY >= menuStartY && touchY <= menuStartY + menuHeight) {

                    float buttonWidth = 16.0f * SCALE;
                    int selectedOption = (touchX - menuStartX) / buttonWidth;

                    if (currentTurn == BLACK) selectedOption = 3 - selectedOption;

                    PieceType newType;
                    if (selectedOption == 0) newType = BISHOP;
                    else if (selectedOption == 1) newType = KNIGHT;
                    else if (selectedOption == 2) newType = ROOK;
                    else newType = QUEEN;

                    chessBoard.setPiece(promoX, promoY, {newType, (PieceColor)currentTurn});

                    isPromoting = false;
                    currentTurn = (currentTurn == WHITE) ? BLACK : WHITE;

                    
                    checkGameEnd = true;
                }
            }
        } else {
            
            int touchX, touchY;
            if (input.getTouchOnBoard(touchX, touchY)) {
                if (selectedX == -1 && selectedY == -1) {
                    Piece p = chessBoard.getPiece(touchX, touchY);
                    if (p.type != EMPTY && p.color == currentTurn) {
                        selectedX = touchX;
                        selectedY = touchY;
                    }
                } else {
                    if (touchX == selectedX && touchY == selectedY) {
                        selectedX = -1;
                        selectedY = -1;
                    } else {
                        if (chessBoard.isValidMove(selectedX, selectedY, touchX, touchY)) {
                            
                            Board boardSnapshot = chessBoard;

                            
                            Piece movingPiece = chessBoard.getPiece(selectedX, selectedY);
                            Piece targetPiece = chessBoard.getPiece(touchX, touchY);

                            chessBoard.setPiece(touchX, touchY, movingPiece);
                            chessBoard.setPiece(selectedX, selectedY, {EMPTY, WHITE});

                            if (chessBoard.isInCheck(currentTurn)) {
                                
                                chessBoard.setPiece(selectedX, selectedY, movingPiece);
                                chessBoard.setPiece(touchX, touchY, targetPiece);
                            } else {
                                
                                boardHistory.push_back(boardSnapshot);
                                turnHistory.push_back(currentTurn);

                                
                                
                                if (movingPiece.type == PAWN && targetPiece.type == EMPTY && touchX != selectedX) {
                                    
                                    chessBoard.setPiece(touchX, selectedY, {EMPTY, WHITE});
                                }

                                movingPiece.hasMoved = true;
                                chessBoard.setPiece(touchX, touchY, movingPiece);

                                
                                if (movingPiece.type == KING && std::abs(touchX - selectedX) == 2) {
                                    int rookStartX = (touchX > selectedX) ? 7 : 0;
                                    int rookEndX = (touchX > selectedX) ? touchX - 1 : touchX + 1;
                                    Piece rook = chessBoard.getPiece(rookStartX, touchY);
                                    rook.hasMoved = true;
                                    chessBoard.setPiece(rookEndX, touchY, rook);
                                    chessBoard.setPiece(rookStartX, touchY, {EMPTY, WHITE});
                                }

                                
                                
                                if (movingPiece.type == PAWN && std::abs(touchY - selectedY) == 2) {
                                    chessBoard.epX = touchX;
                                    chessBoard.epY = (selectedY + touchY) / 2; 
                                } else {
                                    
                                    chessBoard.epX = -1;
                                    chessBoard.epY = -1;
                                }

                                
                                int promotionRow = (currentTurn == WHITE) ? 0 : 7;
                                if (movingPiece.type == PAWN && touchY == promotionRow) {
                                    isPromoting = true;
                                    promoX = touchX;
                                    promoY = touchY;
                                } else {
                                    currentTurn = (currentTurn == WHITE) ? BLACK : WHITE;

                                    
                                    checkGameEnd = true;
                                }
                            }
                        }
                        selectedX = -1;
                        selectedY = -1;
                    }
                }
            }
        }

        
        
        if (checkGameEnd) {
            if (!chessBoard.hasLegalMoves(currentTurn)) {
                if (chessBoard.isInCheck(currentTurn)) {
                    state = (currentTurn == WHITE) ? BLACK_WINS : WHITE_WINS;
                } else {
                    state = DRAW;
                }
            }
        }
        
        vita2d_start_drawing();
        vita2d_clear_screen();

        if (state == MAIN_MENU) {
            renderer.drawMainMenu();
        }
        else if (state == HOW_TO_PLAY) {
            renderer.drawHowToPlay();
        }
        else {
            
            renderer.drawBoard();
            if (selectedX != -1 && selectedY != -1) renderer.drawHighlight(selectedX, selectedY);
            renderer.drawPieces(chessBoard);

            if (isPromoting) {
                renderer.drawPromotionMenu(currentTurn);
            }

            if (state == WHITE_WINS) renderer.drawEndBanner(WHITE);
            else if (state == BLACK_WINS) renderer.drawEndBanner(BLACK);
            else if (state == DRAW) renderer.drawEndBanner(-1);
        }

        vita2d_end_drawing();
        vita2d_swap_buffers();
    }

    vita2d_fini();
    sceKernelExitProcess(0);
    return 0;
}