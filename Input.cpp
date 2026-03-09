#include "input.h"
#include <cmath>

Input::Input() {
    
    sceTouchSetSamplingState(SCE_TOUCH_PORT_FRONT, SCE_TOUCH_SAMPLING_STATE_START);
    wasTouchedLastFrame = false;
}

bool Input::getTouchOnBoard(int& boardX, int& boardY) {
    SceTouchData touch;
    sceTouchPeek(SCE_TOUCH_PORT_FRONT, &touch, 1);

    if (touch.reportNum > 0) {
        if (!wasTouchedLastFrame) {
            wasTouchedLastFrame = true;

            int touchX = touch.report[0].x/2;
            int touchY = touch.report[0].y/2;

            
            int bX = (touchX - BOARD_OFFSET_X) / DRAW_TILE_SIZE;
            int bY = (touchY - BOARD_OFFSET_Y) / DRAW_TILE_SIZE;

            
            if (bX >= 0 && bX < 8 && bY >= 0 && bY < 8) {
                boardX = bX;
                boardY = bY;
                return true;
            }
        }
    } else {
        wasTouchedLastFrame = false;
    }

    return false;
}
bool Input::getRawTouch(int& screenX, int& screenY) {
    SceTouchData touch;
    sceTouchPeek(SCE_TOUCH_PORT_FRONT, &touch, 1);

    if (touch.reportNum > 0) {
        if (!wasTouchedLastFrame) {
            wasTouchedLastFrame = true;

            
            screenX = touch.report[0].x / 2;
            screenY = touch.report[0].y / 2;

            return true;
        }
    } else {
        wasTouchedLastFrame = false; 
    }

    return false;
}

bool Input::isStartPressed() {
    SceCtrlData pad;
    sceCtrlPeekBufferPositive(0, &pad, 1);

    bool pressed = (pad.buttons & SCE_CTRL_START) != 0;
    if (pressed && !startWasPressed) {
        startWasPressed = true;
        return true;
    } else if (!pressed) {
        startWasPressed = false;
    }
    return false;
}

bool Input::isSelectPressed() {
    SceCtrlData pad;
    sceCtrlPeekBufferPositive(0, &pad, 1);

    bool pressed = (pad.buttons & SCE_CTRL_SELECT) != 0;
    if (pressed && !selectWasPressed) {
        selectWasPressed = true;
        return true;
    } else if (!pressed) {
        selectWasPressed = false;
    }
    return false;
}