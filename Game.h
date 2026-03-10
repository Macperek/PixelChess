#pragma once
#include "Board.h"
#include "Renderer.h"
#include "input.h"
#include "Constants.h"
#include <vector>

class Game {
public:
    Game();
    ~Game();
    void run();

private:
    void handleGlobalInput();
    void handleStateInput();
    void handleMainMenuInput();
    void handleHowToPlayInput();
    void handlePlayingInput();
    void handlePromotionInput();
    void performMove(int startX, int startY, int endX, int endY);
    void checkGameEndStatus();
    void render();

    Board chessBoard;
    Renderer renderer;
    Input input;

    int selectedX;
    int selectedY;
    bool isPromoting;
    int promoX;
    int promoY;

    int currentTurn;
    GameState state;

    std::vector<Board> boardHistory;
    std::vector<int> turnHistory;
    bool checkGameEnd;
};