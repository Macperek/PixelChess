#include "Game.h"
#include <vita2d.h>
#include <psp2/kernel/processmgr.h>
#include <cmath>

Game::Game() {
    selectedX = -1;
    selectedY = -1;
    isPromoting = false;
    promoX = -1;
    promoY = -1;

    currentTurn = WHITE;
    state = MAIN_MENU;
    checkGameEnd = false;
}

Game::~Game() {
}

void Game::run() {
    while (true) {
        handleGlobalInput();
        handleStateInput();

        if (checkGameEnd) {
            checkGameEndStatus();
        }

        render();
    }
}

void Game::handleGlobalInput() {
    if (input.isStartPressed()) {
        chessBoard.reset();
        currentTurn = WHITE;
        state = PLAYING;
        selectedX = -1;
        selectedY = -1;
        isPromoting = false;
        boardHistory.clear();
        turnHistory.clear();
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
}

void Game::handleStateInput() {
    checkGameEnd = false;

    if (state == MAIN_MENU) {
        handleMainMenuInput();
    }
    else if (state == HOW_TO_PLAY) {
        handleHowToPlayInput();
    }
    else if (state == WHITE_WINS || state == BLACK_WINS || state == DRAW) {
    }
    else if (isPromoting) {
        handlePromotionInput();
    }
    else {
        handlePlayingInput();
    }
}

void Game::handleMainMenuInput() {
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

void Game::handleHowToPlayInput() {
    int touchX, touchY;
    if (input.getRawTouch(touchX, touchY)) {
        chessBoard.reset();
        boardHistory.clear();
        turnHistory.clear();
        currentTurn = WHITE;
        state = PLAYING;
    }
}

void Game::handlePromotionInput() {
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
}

void Game::handlePlayingInput() {
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
                    performMove(selectedX, selectedY, touchX, touchY);
                }
                selectedX = -1;
                selectedY = -1;
            }
        }
    }
}

void Game::performMove(int startX, int startY, int endX, int endY) {
    Board boardSnapshot = chessBoard;

    Piece movingPiece = chessBoard.getPiece(startX, startY);
    Piece targetPiece = chessBoard.getPiece(endX, endY);

    chessBoard.setPiece(endX, endY, movingPiece);
    chessBoard.setPiece(startX, startY, {EMPTY, WHITE});

    if (chessBoard.isInCheck(currentTurn)) {
        chessBoard.setPiece(startX, startY, movingPiece);
        chessBoard.setPiece(endX, endY, targetPiece);
    } else {
        boardHistory.push_back(boardSnapshot);
        turnHistory.push_back(currentTurn);

        if (movingPiece.type == PAWN && targetPiece.type == EMPTY && endX != startX) {
            chessBoard.setPiece(endX, startY, {EMPTY, WHITE});
        }

        movingPiece.hasMoved = true;
        chessBoard.setPiece(endX, endY, movingPiece);

        if (movingPiece.type == KING && std::abs(endX - startX) == 2) {
            int rookStartX = (endX > startX) ? 7 : 0;
            int rookEndX = (endX > startX) ? endX - 1 : endX + 1;
            Piece rook = chessBoard.getPiece(rookStartX, endY);
            rook.hasMoved = true;
            chessBoard.setPiece(rookEndX, endY, rook);
            chessBoard.setPiece(rookStartX, endY, {EMPTY, WHITE});
        }

        if (movingPiece.type == PAWN && std::abs(endY - startY) == 2) {
            chessBoard.epX = endX;
            chessBoard.epY = (startY + endY) / 2;
        } else {
            chessBoard.epX = -1;
            chessBoard.epY = -1;
        }

        int promotionRow = (currentTurn == WHITE) ? 0 : 7;
        if (movingPiece.type == PAWN && endY == promotionRow) {
            isPromoting = true;
            promoX = endX;
            promoY = endY;
        } else {
            currentTurn = (currentTurn == WHITE) ? BLACK : WHITE;
            checkGameEnd = true;
        }
    }
}

void Game::checkGameEndStatus() {
    if (!chessBoard.hasLegalMoves(currentTurn)) {
        if (chessBoard.isInCheck(currentTurn)) {
            state = (currentTurn == WHITE) ? BLACK_WINS : WHITE_WINS;
        } else {
            state = DRAW;
        }
    }
}

void Game::render() {
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