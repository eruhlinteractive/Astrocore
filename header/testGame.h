#include "../AstroCore/astrocore.h"
#include "testScene.h"


class TestGame : public Game
{
public:

    void InitGame(std::string windowTitle, int screenWidth, int screenHeight)
    {   
        SetConfigFlags(FLAG_WINDOW_RESIZABLE);
        Game::InitGame(windowTitle, screenWidth, screenHeight, 0);
        currentScene = new TestScene();
        //SetTargetFPS(60);
    }
};