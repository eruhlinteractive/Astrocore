#include "../../include/astrocore/systems/game.h"
using namespace Astrocore;

Game::Game(std::string title, int windowWidth, int windowHeight)
{
    Debug::init();
    InitWindow(windowWidth, windowHeight, title.c_str());
    renderer->SetFinalTargetDimensions(windowWidth, windowHeight);
}

void Game::Run()
{
    while(!WindowShouldClose())
	{
        // Update
        sceneTree->GetRoot().lock()->Update(GetFrameTime());
        // Physics Update
        // TODO:
        // Render
       renderer->Render(sceneTree->drawnNodesInScene.get());
    }


    renderer.reset();
    // Cleanup
    CloseWindow();
   
}

Game::~Game()
{
    sceneTree.release();
}