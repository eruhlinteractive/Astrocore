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
#include "testScene.h"
#include "physicsScene.h"
#include "physicsDebug.h"

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    Game *game = &Game::instance();
    int screenWidth = 1280;
    int screenHeight = 720;

    game->InitGame("Astrolib", 1280, 720);

    // Create base scene with new test scene
    game->SetCurrentScene(make_unique<TestScene>());
    game->GetCurrentScene()->GetPhysicsWorld()->SetDebugDraw(new PhysicsDebug());
    Debug::SetDebugFlag(DRAW_PHYSICS_BOUNDS);
    //Debug::SetDebugFlag(DRAW_SPRITE_BOUNDS);

    game->Run();

    delete game;
    game = nullptr;
    return 0;
}