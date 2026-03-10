#pragma once
#include <psp2/touch.h>
#include <psp2/ctrl.h>
#include "Constants.h"

class Input {
public:
    Input();
    bool getTouchOnBoard(int& boardX, int& boardY);
    bool getRawTouch(int& screenX, int& screenY);
    bool isStartPressed();
    bool isSelectPressed();

private:
    bool wasTouchedLastFrame;
    bool startWasPressed;
    bool selectWasPressed;
};