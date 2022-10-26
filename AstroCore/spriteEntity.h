#include <raylib.h>
#include "entity.h"
#include <string>
#include "structs.h"

#ifndef SPRITE_ENTITY
#define SPRITE_ENTITY


using namespace std;

class SpriteEntity: public Entity2D
{

public:
    SpriteEntity(Vector2 origin, Vector2 frameSize, string spriteSrcPath);
    SpriteEntity(Vector2 origin, Vector2 frameSize, string spriteSrcPath, int framesWide, int frameCount, int animFps);


    SpriteEntity(Sprite* sprite);

    /// @brief Flip the sprite horizontally (local)
    void FlipH(){spriteFlip.x *= -1;};
     /// @brief Flip the sprite vertically (local)
    void FlipV(){spriteFlip.y *= -1;};
    void Init();
    virtual void Draw(float frameTime);
    ~SpriteEntity();

private:

    Sprite* sprite;
    string spriteSrcPath;
    Vector2 spriteFlip = {1,1};
    //Texture2D spriteText;
    float animAcc = 0.0f;

    Rectangle srcRect;
    Rectangle destRect;
    
};

#endif // Sprite entity