#include "spriteEntity.h"
#include <iostream>

/// @brief Initialize a static sprite entity
/// @param origin 
/// @param spriteSize 
/// @param spriteSrcPath 
SpriteEntity::SpriteEntity(Vector2 origin, Vector2 frameSize, string spriteSrcPath)
{

    this->spriteSrcPath = spriteSrcPath;
    sprite = new Sprite();

    sprite->origin = origin;
    sprite->frameSize = frameSize;
    sprite->framesWide = 1;

    Init();
}

SpriteEntity::SpriteEntity(Vector2 origin, Vector2 frameSize, string spriteSrcPath, int framesWide, int frameCount, int animFps) : SpriteEntity(origin,frameSize,spriteSrcPath)
{
    sprite->framesWide = framesWide;
    sprite->animFPS = animFps;
    sprite->frameMax = frameCount;
}

SpriteEntity::SpriteEntity(Sprite* sprite)
{
    this->sprite = sprite; 
}

SpriteEntity::~SpriteEntity()
{
    UnloadTexture(sprite->spriteTexture);

    // Cleanup memory
    delete sprite;
    sprite = nullptr;
}

void SpriteEntity::Init()
{
    string pat = string(GetApplicationDirectory()) + spriteSrcPath;
    //std::cout<< pat << endl;
    sprite->spriteTexture = LoadTexture(pat.c_str());
}


void SpriteEntity::Draw(float frameTime)
{
    Rectangle srcRect, destRect;
    float srcPosX, srcPosY;
    animAcc += frameTime;

    if(animAcc >= 1.0/(float)sprite->animFPS)
    {
        sprite->currentFrame++;
        if(sprite->currentFrame >= sprite->frameMax) sprite->currentFrame = 0;
        animAcc = 0.0;
    }


    srcPosX = sprite->currentFrame % sprite->framesWide * sprite->frameSize.x;
    srcPosY = (int)(sprite->currentFrame / sprite->framesWide) * sprite->frameSize.y;

    // Update destination rect
    Vector2 globalPos = this->GetGlobalPosition();

    // Animated sprite
    if(sprite->frameMax != 0)
    {
        srcRect = (Rectangle){srcPosX,srcPosY, sprite->frameSize.x, sprite->frameSize.y};
        destRect = (Rectangle){globalPos.x, globalPos.y, this->sprite->frameSize.x * scaleX, this->sprite->frameSize.x * scaleY};
    }

    // Static Sprite
    else
    {
        srcRect = (Rectangle){0, 0, sprite->frameSize.x, sprite->frameSize.y};
        destRect = (Rectangle){globalPos.x, globalPos.y, this->sprite->frameSize.x * scaleX, this->sprite->frameSize.x * scaleY};
    }

    //DrawTexture(spriteText,positionX,positionY,WHITE);
    DrawTexturePro(sprite->spriteTexture, srcRect, destRect, {sprite->origin.x * scaleX ,sprite->origin.y * scaleY}, rotation * (180.0/PI), WHITE);
}