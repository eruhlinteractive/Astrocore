#include "../header/spriteEntity.h"
using namespace Astrolib;

SpriteEntity::SpriteEntity(Sprite *sprite)
{
    this->sprite = sprite;
    name = "SpriteEntity_" + std::to_string(entityID);
    type = SPRITE;
}

SpriteEntity::SpriteEntity(Texture2D spriteTexture, Vector2 frameSize, Vector2 origin)
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
    return (Rectangle){pos.x, pos.y, sprite->spriteTexture.width, sprite->spriteTexture.height};
}

void SpriteEntity::Draw(float deltaTime, Camera2D *camera)
{
    // Update destination rect
    Rectangle srcRect, destRect;
    Vector2 globalPos = GetGlobalPosition();
    srcRect = (Rectangle){0.0f, 0.0f, (float)sprite->spriteTexture.width, (float)sprite->spriteTexture.height};
    destRect = (Rectangle){globalPos.x, globalPos.y, sprite->spriteTexture.width * transform.scale.x, sprite->spriteTexture.height * transform.scale.y};
    DrawTexturePro(sprite->spriteTexture, srcRect, destRect, {(sprite->origin.x * transform.scale.x),(sprite->origin.y * transform.scale.y)}, GetGlobalRotationDeg(), WHITE);

    if (Debug::instance()->IsDebugFlagSet(DRAW_SPRITE_BOUNDS))
    {
        Rectangle rect = GetBoundRect();
        DrawRectangleLines(destRect.x - (sprite->origin.x * transform.scale.x), destRect.y - (sprite->origin.y * transform.scale.y), destRect.width, destRect.height, RED);
    }
}
