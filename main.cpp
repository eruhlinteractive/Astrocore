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

#include "raylib.h"
#include "header/game.h"
#include <iostream>
#include <string>
#include <math.h>
#include "AstroCore/astrocore.h"

int main()
{
    Game* game = new Game();

   
    const int screenWidth = 1024;
    const int screenHeight = 600;
    float deltaTime = 0.0;

    
    InitWindow(screenWidth, screenHeight, "Agromation 2D :D");

    game->InitGame(screenWidth, screenHeight);

    //testSprite->SetRotation(0.78539);
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

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

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    game->OnExit();
    
    return 0;
}