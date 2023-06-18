#include "scene.h"
#include <vector>
#include <map>
#include "../include/raylib.h"
#include <string>

#ifndef __GAME__
#define __GAME__

using namespace std;

namespace Astrolib
{
    class Game
    {
    public:
        Game(){};

        /// @brief Initialize Runtime
        virtual void InitGame(std::string windowTitle, int screenWidth, int screenHeight, int targetFPS = 60)
        {
            std::string debugMessage = "";

// IDK if this even works with CMake ¯\_(ツ)_/¯
#if __DEBUG__
            debugMessage = " (Debug)";
#else
            debugMessage = " (Release)";
#endif

            InitWindow(screenWidth, screenHeight, "Astrolib Game");
            SetWindowTitle((windowTitle + debugMessage).c_str());
            SetWindowSize(screenWidth, screenHeight);
            SetTargetFPS(targetFPS);
        }

        virtual void CreateScene()
        {
            currentScene = new Scene();
        }

        /// @brief Update the current world
        virtual void Update()
        {
            currentScene->Update(GetFrameTime());
        };

        /// @brief Draw's the world using worldCamera
        virtual void DrawWorld()
        {
            currentScene->Draw(GetFrameTime());
        }

        /// @brief Draw in screen space
        void DrawUI();

        virtual void Update(float deltaTime) { currentScene->Update(GetFrameTime()); };
        virtual void FixedUpdate(float deltaTime) { currentScene->FixedUpdate(deltaTime); };
        virtual void Draw(float deltaTime)
        {
            currentScene->Draw(GetFrameTime());
        };

        Scene *GetCurrentScene()
        {
            return currentScene;
        }

        Vector2 GetScreenSize()
        {
            return (Vector2){(float)GetRenderWidth(), (float)GetRenderHeight()};
        }

        virtual void Exit()
        {
            delete currentScene;
            currentScene = nullptr;

            TextureManager::instance().UnloadAllTextures();
            CloseWindow();
        };

        static Game &instance()
        {
            static Game INSTANCE;
            return INSTANCE;
        }

    protected:
        Scene *currentScene;

    private:
        bool ySortEnabled = true;
    };
}

#endif