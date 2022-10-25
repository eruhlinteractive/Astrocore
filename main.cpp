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
#include "AstroCore/input.h"
#include <string>
#include "AstroCore/spriteEntity.h"

int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1024;
    const int screenHeight = 600;

    // Get reference to input instance
    InputManager input = InputManager::instance();
    
    // TEMP testing 

    InputEvent keyEvent = InputEvent(69);
    std::shared_ptr<InputAction> test = std::make_shared<InputAction>("test");
    std::shared_ptr<InputAction> testTwo = std::make_shared<InputAction>("testTwo");

    testTwo->AddInputEvent(InputEvent(70));

    test->AddInputEvent(keyEvent);
    input.AddInputAction(test);
    input.AddInputAction(testTwo);

    InitWindow(screenWidth, screenHeight, "Agromation");

    SpriteEntity* testSprite = new SpriteEntity({24,24}, {48,48},"../src/res/testSprite.png");

    testSprite->MoveGlobal({screenWidth/2 ,screenHeight/2});
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
        if (input.IsActionPressed("test"))
        {
            //std::cout << "E is pressed!" << std::endl;
            testSprite->RotateDeg(20.0);
        }
        if (input.IsActionPressed("testTwo"))
        {
            //std::cout << "F is NOT pressed!" << std::endl;
            testSprite->RotateDeg(-20.0);
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