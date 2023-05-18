/*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute raylib_compile_execute script
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   Example originally created with raylib 1.0, last time updated with raylib 1.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2013-2022 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include <iostream>
#include <string>
#include <math.h>
#include "AstroCore/astrocore.h"
#include "testGame.h"

int main()
{
    TestGame* game = new TestGame();
    int screenWidth = 1280;
    int screenHeight = 720;

    game->InitGame("Astrolib Dev", 1280, 720);

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        game->Update(GetFrameTime());

        ClearBackground(RAYWHITE);
        
        BeginDrawing();
            
        DrawLine(screenWidth / 2, 0, screenWidth/2, screenHeight, GRAY);
        DrawLine(0, screenHeight/2, screenWidth, screenHeight/2, GRAY);            
        
        game->Draw(GetFrameTime());

        EndDrawing();
    }

    game->Exit();
    delete game;
    game = nullptr;

    return 0;
}