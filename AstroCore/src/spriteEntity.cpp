#include "../header/spriteEntity.h"


SpriteEntity::SpriteEntity(Sprite* sprite)
{
    this->sprite = sprite; 
}

SpriteEntity::SpriteEntity(Texture2D spriteTexture, Vector2 frameSize, Vector2 origin)
{
    this->sprite = new Sprite(spriteTexture, {0,0});
    
}


void SpriteEntity::Draw(float frameTime)
{
    // Update destination rect
    Rectangle srcRect, destRect;
    srcRect = (Rectangle){0, 0, sprite->spriteTexture.width, sprite->spriteTexture.height};
    destRect = (Rectangle){position.x, position.y, sprite->spriteTexture.width * scale.x, sprite->spriteTexture.height * scale.y};
    DrawTexturePro(sprite->spriteTexture, srcRect, destRect, position, rotation, WHITE);
    //DrawTextureEx(sprite->spriteTexture, position, rotation, scale.x, WHITE);

}
