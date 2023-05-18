#include <vector>
#include <map>
#include "../include/raylib.h"
#include <string>

#ifndef __GAME__
#define __GAME__

using namespace std;
using namespace Astrolib;

namespace Astrolib
{
class Game
{
public:

    Game() {};

    /// @brief Initialize Runtime
    virtual void InitGame(std::string windowTitle, int screenWidth, int screenHeight, int targetFPS = 60)
    {
        InitWindow(screenWidth, screenHeight, "Astrolib Dev");
        SetWindowTitle(windowTitle.c_str());
        SetWindowSize(screenWidth, screenHeight);
        SetTargetFPS(targetFPS); 

    }

    virtual void CreateScene(){currentScene = new Scene();}

    /// @brief Update the current world
    virtual void Update(){
        currentScene->Update(GetFrameTime());
    };
    
    /// @brief Draw's the world using worldCamera
    virtual void DrawWorld()
    {
        currentScene->Draw(GetFrameTime());
    }

    /// @brief Draw in screen space
    void DrawUI();

    virtual void Update(float deltaTime){currentScene->Update(GetFrameTime());};
    virtual void Draw(float deltaTime)
    {
        currentScene->Draw(GetFrameTime());
    };

    Scene* currentScene;

    Vector2 GetScreenSize(){return (Vector2){ (float)GetScreenWidth(), (float)GetScreenHeight()};}

    virtual void Exit(){
        delete currentScene;
        currentScene = nullptr;

        TextureManager::instance().UnloadAllTextures();
        CloseWindow();   
    };

    static Game& instance()
    {
        static Game INSTANCE;
        return INSTANCE;
    }

private:
    bool ySortEnabled = true;
};
}

#endif