#include "../AstroCore/astrocore.h"
#include "testScene.h"


class TestGame : public Game
{
public:

    void InitGame(std::string windowTitle, int screenWidth, int screenHeight)
    {   
        Game::InitGame(windowTitle, screenWidth, screenHeight);
        currentScene = new TestScene();
    }
};