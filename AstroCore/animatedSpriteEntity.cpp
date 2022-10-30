#include "animatedSpriteEntity.h"
#include <iostream>

/// @brief Initialize a static sprite entity
/// @param origin 
/// @param spriteSize 
/// @param spriteSrcPath 
AnimatedSpriteEntity::AnimatedSpriteEntity(Vector2 origin, Vector2 frameSize, Texture2D sprite, string defaultAnimName)
{
    Init();
    //this->spriteSrcPath = spriteSrcPath;
    SpriteAnimation* anim = new SpriteAnimation();

    anim->spriteTexture = sprite;
    anim->origin = origin;
    anim->frameSize = frameSize;
    anim->framesWide = 1;

    animStates->insert(pair<string, SpriteAnimation*>(defaultAnimName, anim));
    currentAnimationName = defaultAnimName;
    currentAnim = anim;
}

AnimatedSpriteEntity::AnimatedSpriteEntity(Vector2 origin, Vector2 frameSize, Texture2D sprite, int framesWide, int frameCount, int animFps) : AnimatedSpriteEntity(origin,frameSize, sprite)
{
    SpriteAnimation* anim = (*animStates)[currentAnimationName];
    anim->framesWide = framesWide;
    anim->animFPS = animFps;
    anim->frameMax = frameCount;
}

AnimatedSpriteEntity::AnimatedSpriteEntity(SpriteAnimation* defaultAnimation, string animationName)
{
    Init();
    animStates->insert(pair<string, SpriteAnimation*>(animationName, defaultAnimation));
    currentAnimationName = animationName;
}

void AnimatedSpriteEntity::ChangeAnimation(string animationName)
{
    if(animStates->find(animationName) != animStates->end())
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
    for(auto anim = animStates->begin(); anim != animStates->end(); ++anim)
    {
        // Unload all textures
        SpriteAnimation* sAnim = anim->second;
        UnloadTexture(sAnim->spriteTexture);
        delete sAnim;
        sAnim = nullptr;
    }

    delete animStates;
    animStates = nullptr;
}

void AnimatedSpriteEntity::Init()
{
    animStates = new std::map<string,SpriteAnimation*>();
}


/// @brief Draw the animated sprite, incrementing the frame counter
/// @param frameTime 
void AnimatedSpriteEntity::Draw(float frameTime)
{
    Rectangle srcRect, destRect;
    float srcPosX, srcPosY;


    if(isPlaying) animAcc += frameTime;

    if(animAcc >= 1.0/(float)currentAnim->animFPS)
    {
        currentAnim->currentFrame++;
        if(currentAnim->currentFrame >= currentAnim->frameMax) currentAnim->currentFrame = 0;
        animAcc = 0.0;
    }
    
    srcPosX = currentAnim->currentFrame % currentAnim->framesWide * currentAnim->frameSize.x;
    srcPosY = (int)(currentAnim->currentFrame / currentAnim->framesWide) * currentAnim->frameSize.y;

    // Update destination rect
    Vector2 globalPos = this->GetGlobalPosition();

    // Animated sprite
    if(currentAnim->frameMax != 0)
    {
        srcRect = (Rectangle){srcPosX,srcPosY, currentAnim->frameSize.x, currentAnim->frameSize.y};
        destRect = (Rectangle){globalPos.x, globalPos.y, currentAnim->frameSize.x * scaleX, currentAnim->frameSize.x * scaleY};
    }

    // Static Sprite
    else
    {
        srcRect = (Rectangle){0, 0, currentAnim->frameSize.x, currentAnim->frameSize.y};
        destRect = (Rectangle){globalPos.x, globalPos.y, currentAnim->frameSize.x * scaleX, currentAnim->frameSize.x * scaleY};
    }

    //DrawTexture(spriteText,positionX,positionY,WHITE);
    DrawTexturePro(currentAnim->spriteTexture, srcRect, destRect, {currentAnim->origin.x * scaleX ,currentAnim->origin.y * scaleY}, rotation * (180.0/PI), WHITE);
}