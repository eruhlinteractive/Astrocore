#include "spriteEntity.h"
using namespace Astrolib;

SpriteEntity::SpriteEntity(Sprite *sprite)
{
    this->sprite = sprite;
    
    name = "SpriteEntity_" + std::to_string(entityID);
    type = SPRITE;
}

SpriteEntity::SpriteEntity(Texture2D* spriteTexture, Vector2 frameSize, Vector2 origin)
{
    this->sprite = new Sprite(spriteTexture, origin);
    name = "SpriteEntity_" + std::to_string(entityID);
    type = SPRITE;
}



SpriteEntity::~SpriteEntity()
{
    TextureManager::instance().UnloadTexture(sprite->spriteTexture);
    delete sprite;
    sprite = nullptr;
}

Rectangle SpriteEntity::GetBoundRect()
{
    Vector2 pos = GetGlobalPosition();
    return (Rectangle){pos.x, pos.y, sprite->spriteTexture->width, sprite->spriteTexture->height};
}

void SpriteEntity::Draw(float deltaTime, Camera2D *camera)
{
    // Update destination rect
    Rectangle srcRect, destRect;
    Vector2 globalPos = GetGlobalPosition();
    srcRect = (Rectangle){sprite->startPoint.x, sprite->startPoint.y, (float)sprite->spriteSize.x, (float)sprite->spriteSize.y};
    destRect = (Rectangle){globalPos.x, globalPos.y, sprite->spriteSize.x * transform.scale.x, sprite->spriteSize.y * transform.scale.y};
    DrawTexturePro(*(sprite->spriteTexture), srcRect, destRect, {(sprite->origin.x * transform.scale.x), (sprite->origin.y * transform.scale.y)}, GetGlobalRotationDeg(), WHITE);

    if (Debug::instance()->IsDebugFlagSet(DRAW_SPRITE_BOUNDS))
    {
        Rectangle rect = GetBoundRect();
        DrawRectangleLines(destRect.x - (sprite->origin.x * transform.scale.x), destRect.y - (sprite->origin.y * transform.scale.y), destRect.width, destRect.height, RED);
    }
}
