#include "../header/animatedSpriteEntity.h"
#include "../header/texturemanager.h"
#include <iostream>

using namespace Astrolib;

#pragma region Constructors

AnimatedSpriteEntity::AnimatedSpriteEntity()
{
    type = SPRITE;
    name = "AnimatedSpriteEntity" + std::to_string(entityID);
}

/// @brief Initialize a static sprite entity
/// @param origin
/// @param spriteSize
/// @param spriteSrcPath
AnimatedSpriteEntity::AnimatedSpriteEntity(std::string name, Vector2 startPos, Vector2 origin, Vector2 frameSize, Texture2D sprite, string defaultAnimName) : AnimatedSpriteEntity()
{
    Init();

    // this->spriteSrcPath = spriteSrcPath;
    SpriteAnimation *anim = new SpriteAnimation();

    anim->spriteTexture = sprite;
    anim->origin = origin;
    anim->frameSize = frameSize;
    anim->framesWide = 1;
    anim->startPos = startPos;
    if (name != "")
    {
        this->name = name;
    }

    animStates->insert(pair<string, SpriteAnimation *>(defaultAnimName, anim));
    currentAnimationName = defaultAnimName;
    currentAnim = anim;
}

AnimatedSpriteEntity::AnimatedSpriteEntity(std::string name, Vector2 startPos, Vector2 origin, Vector2 frameSize, Texture2D sprite, int framesWide, int frameCount, int animFps)
    : AnimatedSpriteEntity(name, startPos, origin, frameSize, sprite)
{
    SpriteAnimation *anim = (*animStates)[currentAnimationName];
    anim->framesWide = framesWide;
    anim->animFPS = animFps;
    anim->frameMax = frameCount;
}

AnimatedSpriteEntity::AnimatedSpriteEntity(std::string name, SpriteAnimation *defaultAnimation, string animationName) : AnimatedSpriteEntity()
{
    Init();
    if (name != "")
    {
        this->name = name;
    }

    animStates->insert(pair<string, SpriteAnimation *>(animationName, defaultAnimation));
    currentAnimationName = animationName;
    currentAnim = defaultAnimation;
}

#pragma endregion

/// @brief Add an animation to the lists of animations for this sprite
/// @param animationName The name of the animation to add
/// @param anim A pointer to the aniamtion
void AnimatedSpriteEntity::AddAnimation(string animationName, SpriteAnimation *anim)
{
    // If it doesn't already exist in the animtion states
    if (animStates->find(animationName) == animStates->end())
    {
        animStates->insert(pair<string, SpriteAnimation *>(animationName, anim));
    }
}

/// @brief Remove an animation from the list of playable animations
/// @param animationName The name of the animation to remove
void AnimatedSpriteEntity::RemoveAnimation(string animationName)
{
    // If the animation exists in the list of animations
    if (animStates->find(animationName) != animStates->end())
    {
        SpriteAnimation *anim = (*animStates)[animationName];
        animStates->erase(animationName);

        delete anim;
        anim = nullptr;
    }
}

/// @brief Change the animation to a new animation state
/// @param animationName The name of the animation to switch to
void AnimatedSpriteEntity::ChangeAnimation(string animationName)
{

    if (animStates->find(animationName) != animStates->end() && animationName != currentAnimationName)
    {
        currentAnimationName = animationName;
        currentAnim = (*animStates)[animationName];

        // Reset frame counter
        animAcc = 0.0;
        currentAnim->currentFrame = 0;
    }
}

AnimatedSpriteEntity::~AnimatedSpriteEntity()
{
    for (auto anim = animStates->begin(); anim != animStates->end(); ++anim)
    {
        // Unload all textures
        SpriteAnimation *sAnim = anim->second;

        if (sAnim != nullptr)
        {
            Texture2D s = sAnim->spriteTexture;
            TextureManager::instance().UnloadTexture(s);

            delete sAnim;
            sAnim = nullptr;
        }
    }

    delete animStates;
    animStates = nullptr;
}

void AnimatedSpriteEntity::Init()
{
    animStates = new std::map<string, SpriteAnimation *>();
}

Rectangle AnimatedSpriteEntity::GetBoundRect()
{
    Vector2 pos = GetGlobalPosition();
    return (Rectangle){pos.x, pos.y, currentAnim->frameSize.x, currentAnim->frameSize.y};
}

/// @brief Draw the animated sprite, incrementing the frame counter
/// @param frameTime
void AnimatedSpriteEntity::Draw(float deltaTime, Camera2D *camera)
{
    Rectangle srcRect, destRect;
    float srcPosX, srcPosY;

    if (isPlaying)
        animAcc += deltaTime;

    if (animAcc >= 1.0 / (float)currentAnim->animFPS)
    {
        currentAnim->currentFrame++;
        if (currentAnim->currentFrame >= currentAnim->frameMax)
            currentAnim->currentFrame = 0;
        animAcc = 0.0;
    }

    srcPosX = currentAnim->currentFrame % currentAnim->framesWide * currentAnim->frameSize.x + currentAnim->startPos.x;
    srcPosY = (int)(currentAnim->currentFrame / currentAnim->framesWide) * currentAnim->frameSize.y + currentAnim->startPos.y;

    // Update destination rect
    Vector2 globalPos = this->GetGlobalPosition();
    //globalPos = (Vector2){GetNearestMultiple(globalPos.x, Entity2D::pixelsPerUnit), GetNearestMultiple(globalPos.y, Entity2D::pixelsPerUnit)};
    // Animated sprite
    //SetTextureFilter(currentAnim->spriteTexture, TEXTURE_FILTER_POINT);
    srcRect = (Rectangle){srcPosX, srcPosY, spriteFlip.x * currentAnim->frameSize.x, spriteFlip.y * currentAnim->frameSize.y};
    destRect = (Rectangle){globalPos.x, globalPos.y, currentAnim->frameSize.x * transform.scale.x, currentAnim->frameSize.x * transform.scale.y};
    // DrawTexture(currentAnim->spriteTexture, srcPosX, srcPosY, WHITE);
    //  DrawTexture(spriteText,positionX,positionY,WHITE);
    
    DrawTexturePro(currentAnim->spriteTexture,
                   srcRect,
                   destRect,
                   {currentAnim->origin.x * transform.scale.x, currentAnim->origin.y * transform.scale.y},
                   GetGlobalRotationDeg(),
                   WHITE);
    

    //DrawTexturePro(currentAnim->spriteTexture,
    //               srcRect,
    //               destRect,
    //               {0,0},
    //               GetGlobalRotation() * (180.0 / PI),
    //               WHITE);
    //DrawRectangleLines(round(destRect.x), round(destRect.y), 1, 1, RED);

    if (Debug::instance()->IsDebugFlagSet(DRAW_SPRITE_BOUNDS))
    {
        DrawRectangleLines(
            destRect.x - destRect.width / 2.0f,
            destRect.y - destRect.height / 2.0f,
            destRect.width,
            destRect.height,
            RED
        );
    }
}