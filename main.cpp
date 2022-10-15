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
#include "input.h"
#include <string>


//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------


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
    InputAction* test = new InputAction("test");
    InputAction* test2 = new InputAction("test2");

    test2->AddInputEvent(InputEvent(70));

    test->AddInputEvent(keyEvent);
    input.AddInputAction(test);
    input.AddInputAction(test2);

    InitWindow(screenWidth, screenHeight, "Agriculture");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        if (input.IsActionPressed("test"))
        {
            std::cout << "E is pressed!" << std::endl;
        }
        else if (input.IsActionPressed("test2"))
        {
            std::cout << "F is NOT pressed!" << std::endl;
        }

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}