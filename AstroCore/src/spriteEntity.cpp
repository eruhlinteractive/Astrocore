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


    DrawTextureEx(sprite->spriteTexture, {positionX ,positionY }, rotation, scaleX, WHITE);

}
