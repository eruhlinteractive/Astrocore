#include "game.h"
#include "testScene.h"
#include "../AstroCore/astrocore.h"

using namespace Astrolib;

void Game::InitGame(int screenWidth, int screenHeight)
{
    //worldCamera->target = testSprite->GetGlobalPosition();
    currentScene = new TestScene();
}


void Game::OnExit()
{
    delete currentScene;
    currentScene = nullptr;
    
    TextureManager::instance().UnloadAllTextures();
}

void Game::Update(float deltaTime)
{
    currentScene->Update(deltaTime);
}

#pragma region Drawing Logic
void Game::Draw(float deltaTime)
{
    // Draw world
    currentScene->Draw(deltaTime);
    DrawUI();
}

void Game::DrawUI()
{
    DrawFPS(10,10);
}
#pragma endregion