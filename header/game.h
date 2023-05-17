#include <vector>
#include <map>
#include "../include/raylib.h"
#include "../AstroCore/astrocore.h"

using namespace std;
using namespace Astrolib;

namespace Astrolib
{
class Game
{
public:

    Game() {};

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

    Scene* currentScene;

    Vector2 GetScreenSize(){return (Vector2){ (float)GetScreenWidth(), (float)GetScreenHeight()};}

    void OnExit();

    static Game& instance()
    {
        static Game INSTANCE;
        return INSTANCE;
    }

private:
    bool ySortEnabled = true;
};
}