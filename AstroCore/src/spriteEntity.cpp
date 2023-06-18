#include "../header/spriteEntity.h"
using namespace Astrolib;

SpriteEntity::SpriteEntity(Sprite *sprite)
{
    this->sprite = sprite;
}

SpriteEntity::SpriteEntity(Texture2D spriteTexture, Vector2 frameSize, Vector2 origin)
{
    this->sprite = new Sprite(spriteTexture, origin);
}

SpriteEntity::~SpriteEntity()
{
    TextureManager::instance().UnloadTexture(sprite->spriteTexture);
    delete sprite;
    sprite = nullptr;
}

Rectangle SpriteEntity::GetSpriteRect()
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
    destRect = (Rectangle){globalPos.x - (sprite->origin.x * transform.scale.x), globalPos.y - (sprite->origin.y * transform.scale.y), sprite->spriteTexture.width * transform.scale.x, sprite->spriteTexture.height * transform.scale.y};
    DrawTexturePro(sprite->spriteTexture, srcRect, destRect, globalPos, GetGlobalRotation(), WHITE);
}
