cmake_minimum_required(VERSION 3.10)
project(PixelChess C CXX)
set(CMAKE_BUILD_TYPE Release)

include("$ENV{VITASDK}/share/vita.cmake")

add_executable(PixelChess main.cpp
        Constants.h
        Board.h
        Renderer.h
        Board.cpp
        Renderer.cpp
        Input.cpp
        input.h)

target_link_libraries(PixelChess
        vita2d
        png
        z
        SceDisplay_stub
        SceCtrl_stub
        SceTouch_stub
        SceGxm_stub
        SceCommonDialog_stub
        SceSysmodule_stub
        SceAppMgr_stub
        ScePgf_stub
        m
)

vita_create_self(eboot.bin PixelChess)

vita_create_vpk(PixelChess.vpk "PIXC02137" eboot.bin
        NAME "Pixel Chess"
        VERSION "01.00"
        FILE ${CMAKE_CURRENT_SOURCE_DIR}/assets assets
        FILE ${CMAKE_CURRENT_SOURCE_DIR}/icon0.png sce_sys/icon0.png
        FILE ${CMAKE_CURRENT_SOURCE_DIR}/bg0.png sce_sys/livearea/contents/bg0.png
        FILE ${CMAKE_CURRENT_SOURCE_DIR}/template.xml sce_sys/livearea/contents/template.xml
)