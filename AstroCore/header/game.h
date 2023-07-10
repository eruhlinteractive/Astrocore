#include "scene.h"
#include <vector>
#include <map>
#include "../include/raylib.h"
#include <string>
#include "../include/box2d/box2d.h"

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
#else
            debugMessage = " (Release)";
#endif

            InitWindow(screenWidth, screenHeight, "Astrolib Game");
            SetWindowTitle((windowTitle + debugMessage).c_str());
            SetWindowSize(screenWidth, screenHeight);
            // SetTargetFPS(targetFPS);
        }

        virtual void CreateScene()
        {
            currentScene = new Scene();
        }

        virtual void LateUpdate()
        {
            currentScene->LateUpdate(GetFrameTime());
        };

        /// @brief Draw's the world using worldCamera
        virtual void DrawWorld()
        {
            currentScene->Draw(GetFrameTime());
        }

        /// @brief Draw in screen space
        void DrawUI();

        virtual void Update(float deltaTime)
        {
            if (IsWindowResized())
            {
                SendEvent("windowResized");
            }
            currentScene->Update(GetFrameTime());
        };
        virtual void FixedUpdate(float deltaTime)
        {
            // TODO: Make this settable by the end user
            physicsWorld->Step(deltaTime, 8, 3);
            currentScene->FixedUpdate(deltaTime);
        };
        virtual void Draw(float deltaTime)
        {
            currentScene->Draw(GetFrameTime());
        };

        Scene *GetCurrentScene()
        {
            return currentScene;
        }
        void SetCurrentScene(Scene *newScene)
        {
            currentScene = newScene;
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
            delete currentScene;
            currentScene = nullptr;

            TextureManager::instance().UnloadAllTextures();
            CloseWindow();
        };

        static b2World *GetPhysicsWorld()
        {
            return physicsWorld;
        }

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
        Scene *currentScene;
        Game()
        {
            physicsWorld = new b2World(b2Vec2(0, 0));
        };

    private:
        inline static Game *INSTANCE;
        inline static bool created;
        bool ySortEnabled = true;
        inline static b2World *physicsWorld;
    };
}

#endif