#include "Game.h"
#include <vita2d.h>
#include <psp2/kernel/processmgr.h>

int main() {
    vita2d_init();
    vita2d_set_clear_color(RGBA8(40, 40, 40, 255));

    Game game;
    game.run();

    vita2d_fini();
    sceKernelExitProcess(0);
    return 0;
}