#include <vector>
#include <map>
#include "../include/raylib.h"
#include "../AstroCore/astrocore.h"

using namespace std;

class Game
{
public:

    Game() {};

    Camera2D *worldCamera;

    /// @brief Initialize Runtime
    void InitGame(int screenWidth, int screenHeight);

    /// @brief Update the current world
    void Update();
    
    /// @brief Draw's the world using worldCamera
    void DrawWorld();

    /// @brief Draw in screen space
    void DrawUI();

    void Update(float deltaTime);
    void Draw(float deltaTime);

    AnimatedSpriteEntity* testSprite;
    AnimatedSpriteEntity* animTest;
    SpriteEntity* testSpr;

    void OnExit();

private:
    std::vector<Entity2D> entities;
};