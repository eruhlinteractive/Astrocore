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

// IDK if this even works with CMake ¯\_(ツ)_/¯
#if __DEBUG__
            debugMessage = " (Debug)";
            2
#else
            debugMessage = " (Release)";
#endif

            InitWindow(screenWidth, screenHeight, "Astrolib Game");
            SetWindowTitle((windowTitle + debugMessage).c_str());
            SetWindowSize(screenWidth, screenHeight);
            SetTargetFPS(targetFPS);
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
            // TODO: Make this settable by the end user
            // physicsWorld->Step(deltaTime, 8, 3);
            currentScene->FixedUpdate(deltaTime);
        };

        // Runs the main game loop
        void Run()
        {
            // TODO: Put core game loop here

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
        }

        void SetConfigFlag(unsigned int flag)
        {
            SetConfigFlags(flag);
        }

        Vector2 GetScreenSize()
        {
            return (Vector2){(float)GetRenderWidth(), (float)GetRenderHeight()};
        }

        virtual void Exit()
        {

            TextureManager::instance().UnloadAllTextures();
            CloseWindow();
        };

        static Game &instance()
        {
            if (!Game::created)
            {
                Game::INSTANCE = new Game();
                Game::created = true;
            }

            return *INSTANCE;
        }

    protected:
        inline static unique_ptr<Scene> currentScene;
        Game(){};

    private:
        inline static Game *INSTANCE;
        inline static bool created;
        bool ySortEnabled = true;
        const float FIXED_UPDATE_RATE = 1.0f / 60.0f;
        float fixedUpdateTimer = 0.0f;
    };
}

#endif