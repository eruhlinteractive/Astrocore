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
#include "game.h"
#include <iostream>
//#include "AstroCore/input.h"
#include <string>
#include <math.h>
#include "AstroCore/astrocore.h"

int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1024;
    const int screenHeight = 600;

    // Get reference to input instance
    InputManager input = InputManager::instance();
    
    // TEMP testing 

    InputEvent keyEvent = InputEvent(KEY_E);
    std::shared_ptr<InputAction> right = std::make_shared<InputAction>("rotRight");
   

    right->AddInputEvent(InputEvent(KEY_Q));

    std::shared_ptr<InputAction> left = std::make_shared<InputAction>("rotLeft");
    left->AddInputEvent(keyEvent);
    input.AddInputAction(right);
    input.AddInputAction(left);

    InitWindow(screenWidth, screenHeight, "Agromation");

    SpriteEntity* testSprite = new SpriteEntity({24,24}, {48,48},"../src/res/testSprite.png");

    testSprite->MoveGlobal({screenWidth/2 ,screenHeight/2});

    std::shared_ptr<InputAction> up = std::make_shared<InputAction>("up");
    up->AddInputEvent(InputEvent(KEY_W));
    input.AddInputAction(up);

    //testSprite->SetRotation(0.78539);
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------
        DrawFPS(10,10);
        if (input.IsActionPressed("rotRight"))
        {
            //std::cout << "E is pressed!" << std::endl;
            testSprite->RotateDeg(-1);
        }
        if (input.IsActionPressed("rotLeft"))
        {
            //std::cout << "F is NOT pressed!" << std::endl;
            testSprite->RotateDeg(1);
        }

        if(input.IsActionPressed("up"))
        {
            testSprite->MoveLocal({0,50 * GetFrameTime()});
        }
        //testSprite->MoveLocal({5.0 * GetFrameTime(), 5.0 * GetFrameTime()});
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
            testSprite->Draw();


        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    return 0;
}