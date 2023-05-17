#include "../header/spriteEntity.h"
using namespace Astrolib;

SpriteEntity::SpriteEntity(Sprite* sprite)
{
    this->sprite = sprite; 
}

SpriteEntity::SpriteEntity(Texture2D spriteTexture, Vector2 frameSize, Vector2 origin)
{
    this->sprite = new Sprite(spriteTexture, origin);
    
}


void SpriteEntity::Draw(float deltaTime)
{
    // Update destination rect
    Rectangle srcRect, destRect;
    srcRect = (Rectangle){0.0f, 0.0f, (float)sprite->spriteTexture.width, (float)sprite->spriteTexture.height};
    destRect = (Rectangle){position.x - (sprite->origin.x * scale.x), position.y- (sprite->origin.y * scale.y), sprite->spriteTexture.width * scale.x, sprite->spriteTexture.height * scale.y};
    DrawTexturePro(sprite->spriteTexture, srcRect, destRect, position, rotation, WHITE);
}
