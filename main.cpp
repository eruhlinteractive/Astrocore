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
    const int screenWidth = 1024;
    const int screenHeight = 600;
    float deltaTime = 0.0;

    // Get reference to input instance
    InputManager input = InputManager::instance();
    
    // Init testing input map
    std::shared_ptr<InputAction> right = std::make_shared<InputAction>("right",KEY_D);
    std::shared_ptr<InputAction> left = std::make_shared<InputAction>("left",KEY_A);
    std::shared_ptr<InputAction> up = std::make_shared<InputAction>("up",KEY_W);
    std::shared_ptr<InputAction> down = std::make_shared<InputAction>("down",KEY_S);

    std::shared_ptr<InputAction> rotCW = std::make_shared<InputAction>("rotCW",KEY_E);
    std::shared_ptr<InputAction> rotCCW = std::make_shared<InputAction>("rotCCW",KEY_Q);
   
    input.AddInputAction(right);
    input.AddInputAction(left);
    input.AddInputAction(up);
    input.AddInputAction(down);
    input.AddInputAction(rotCW);
    input.AddInputAction(rotCCW);



    InitWindow(screenWidth, screenHeight, "Agromation");

    Texture2D spriteRun = LoadTexture("../src/res/anim_test.png");
    Texture2D spriteIdle = LoadTexture("../src/res/idle_test.png");

    AnimatedSpriteEntity* testSprite = new AnimatedSpriteEntity(
        {25,18}, 
        {50,37},
        spriteRun,
        6,
        6,
        8
        );

    SpriteAnimation* idleAnim = new SpriteAnimation(spriteIdle, 4, 6, 4, {25,18}, {50,37});

    testSprite->AddAnimation("idle", idleAnim);

    AnimatedSpriteEntity* testChild = new AnimatedSpriteEntity({24,24}, {48,48}, spriteIdle);

    AnimatedSpriteEntity* animTest = new AnimatedSpriteEntity(
        {25,18}, 
        {50,37},
        spriteRun,
        6,
        6,
        20
        );

    testChild->SetScale({0.5,0.5});
    animTest->SetScale({4,4});

    //testChild->MoveGlobal({screenWidth/2.0,screenHeight/2.0});
    testChild->SetPosition({50,0});
    testSprite->AddChild(testChild);
    
    animTest->SetScale({5,5});
    animTest->MoveGlobal({screenWidth/4 ,screenHeight/4 });
    testSprite->MoveGlobal({screenWidth/2 ,screenHeight/2 });
    //testSprite->RotateDeg(45);

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

        

        if (input.IsActionDown("right"))
        {
            testSprite->MoveLocal({100 * GetFrameTime(),0});
            testSprite->ChangeAnimation("default");
            testSprite->SetFlipped(false, false);
            animTest->SetFlipped(false, false);
        }
        else if(input.IsActionDown("left"))
        {
            testSprite->MoveLocal({-100 * GetFrameTime(),0});
            testSprite->ChangeAnimation("default");
            testSprite->SetFlipped(true, false);
            animTest->SetFlipped(true, false);
        }
        else
        {
            testSprite->ChangeAnimation("idle");
            //testSprite->SetFlipped(false, false);
            animTest->SetFlipped(false, false);
        }
        

        if(input.IsActionDown("up"))
        {
            testSprite->MoveLocal({0,-50 * GetFrameTime()});
        }
        if(input.IsActionDown("down"))
        {
            testSprite->MoveLocal({0, 50 * GetFrameTime()});
        }
        //if(input.IsActionDown("rotCW"))
        //{
        //    testSprite->RotateDeg(1);
        //}
        //if(input.IsActionDown("rotCCW"))
        //{
        //    testSprite->RotateDeg(-1);
        //}

        deltaTime = GetFrameTime();
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawLine(screenWidth / 2, 0, screenWidth/2, screenHeight, GRAY);
            DrawLine(0, screenHeight/2, screenWidth, screenHeight/2, GRAY);

            testSprite->Draw(deltaTime);
            //testChild->Draw(deltaTime);
            //animTest->Draw(deltaTime);
            
        EndDrawing();
        //----------------------------------------------------------------------------------
        
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    return 0;
}