#include <raylib.h>
#include "entity.h"
#include "spriteEntity.h"
#include <string>
#include "structs.h"
#include <map>
#include "interfaces.h"

#ifndef ANIMATED_SPRITE_ENTITY_H
#define ANIMATED_SPRITE_ENTITY_H


using namespace std;

namespace Astrolib
{
class AnimatedSpriteEntity: public Entity2D
{

public:
    AnimatedSpriteEntity();
    AnimatedSpriteEntity(std::string name, Vector2 origin, Vector2 frameSize, Texture2D sprite, string defaultAnimName = "default");
    AnimatedSpriteEntity(std::string name, Vector2 origin, Vector2 frameSize, Texture2D sprite, int framesWide, int frameCount, int animFps);
    AnimatedSpriteEntity(std::string name, SpriteAnimation* defaultAnimation, string animationName);


    void AddAnimation(string animationName, SpriteAnimation* anim);
    void RemoveAnimation(string animationName);
    bool HasAnimation(string animationName);

    void ChangeAnimation(string animationName);
    void PauseAnimation(){isPlaying = false;};
    void PlayAnimation(){isPlaying = true;};
    string GetCurrentAnimation(){return currentAnimationName;};
    void Draw(float deltaTime, Camera2D* camera) override;
    Rectangle GetSpriteRect() override;
    void Init();

    bool IsFlippedH(){return (spriteFlip.x == -1);};
    bool IsFlippedV(){return (spriteFlip.y == -1);};
    void SetFlipped(bool hFlipped, bool vFlipped)
    { spriteFlip.y = vFlipped ? -1:1; spriteFlip.x = hFlipped ? -1:1;};

    /// @brief Flip the sprite horizontally (local)
    void FlipH(){spriteFlip.x *= -1;};
    
    /// @brief Flip the sprite vertically (local)
    void FlipV(){spriteFlip.y *= -1;};


    // TODO: Implement Rule of Three
    ~AnimatedSpriteEntity();

private:

    string currentAnimationName;
    Vector2 spriteFlip = {1,1};
    
    float animAcc = 0.0f;
    SpriteAnimation* currentAnim;

    /// @brief Is the animation currently playing
    bool isPlaying = true;

    map<string,SpriteAnimation*>* animStates;
    
};

}
#endif // Animated Sprite entity