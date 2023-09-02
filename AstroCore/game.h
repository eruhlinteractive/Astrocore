#include "engine/scene.h"
#include <vector>
#include <map>
#include "../include/raylib.h"
#include <string>
#include "include/box2d/box2d.h"

#ifndef __GAME__
#define __GAME__

using namespace std;

namespace Astrolib
{
    class Game : public Signaler
    {
    public:
        /// @brief Initialize Runtime
        virtual void InitGame(std::string windowTitle, int screenWidth, int screenHeight, int targetFPS = 60)
        {
            std::string debugMessage = "";

            InitWindow(screenWidth, screenHeight, "Astrolib Game");
            SetWindowTitle((windowTitle + debugMessage).c_str());
            SetWindowSize(screenWidth, screenHeight);
            SetTargetFPS(targetFPS);
             
            //SetWindowPosition(GetMonitorWidth(GetCurrentMonitor())/2 + screenWidth/5, GetMonitorHeight(GetCurrentMonitor())/2 - screenHeight/2);
        }

        void LateUpdate()
        {
            currentScene->LateUpdate(GetFrameTime());
        };

        /// @brief Draw's the world using worldCamera
        void DrawWorld()
        {
            currentScene->Draw(GetFrameTime());
        }

        /// @brief Draw in screen space
        void DrawUI();

        /// @brief Called once per frame on the current scene
        /// @param deltaTime 
        void Update(float deltaTime)
        {
            if (IsWindowResized())
            {
                SendEvent("windowResized");
            }

            currentScene->Update(GetFrameTime());
        };
        
        void FixedUpdate(float deltaTime)
        {
            // Run physics updates for the current scene
            currentScene->FixedUpdate(deltaTime);
        };

        // Runs the main game loop
        void Run()
        {
            // Core game loop

            while (!WindowShouldClose())
            {
                float x = GetFrameTime();
                Update(GetFrameTime());

                LateUpdate();
                // Run FixedUpdate()
                fixedUpdateTimer += GetFrameTime();
                while (fixedUpdateTimer > FIXED_UPDATE_RATE)
                {
                    fixedUpdateTimer -= FIXED_UPDATE_RATE;
                    FixedUpdate(FIXED_UPDATE_RATE);
                }

                ClearBackground(WHITE);
                Draw(GetFrameTime());
            }

            // Run cleanup
            Exit();
        };

        void Draw(float deltaTime)
        {
            currentScene->Draw(GetFrameTime());
        };

        static Scene *GetCurrentScene()
        {
            return currentScene.get();
        }

        /// @brief Set the current scene of the game, transfering ownership to the Game instance
        /// @param newScene A unique pointer to the new scene
        void SetCurrentScene(unique_ptr<Scene> newScene)
        {
            currentScene = std::move(newScene);
            AddObserver(currentScene.get(), "windowResized");
            currentScene->LoadScene();

        }

        void SetConfigFlag(unsigned int flag)
        {
            SetConfigFlags(flag);
        }

        Vector2 GetScreenSize()
        {
            return (Vector2){(float)GetRenderWidth(), (float)GetRenderHeight()};
        }

        /// @brief Exits the game and closes the window
        void Exit()
        {
            // Delete the current scene
            currentScene.reset();

            TextureManager::instance().UnloadAllTextures();
            CloseWindow();
        };

        static Game &instance()
        {
            if (!Game::isInitialized)
            {
                Game::INSTANCE = new Game();
                Game::isInitialized = true;
            }

            return *INSTANCE;
        }

    private:
        inline static unique_ptr<Scene> currentScene;
        Game(){};
        inline static Game *INSTANCE;
        inline static bool isInitialized;
        bool ySortEnabled = true;
        const float FIXED_UPDATE_RATE = 1.0f / 60.0f;
        float fixedUpdateTimer = 0.0f;
    };
}

#endif