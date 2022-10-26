#include "spriteEntity.h"
#include <iostream>

SpriteEntity::SpriteEntity(string spriteSrcPath)
{
    this->spriteSrcPath = spriteSrcPath;
    Init();
}
SpriteEntity::SpriteEntity(Vector2 origin, Vector2 spriteSize, string spriteSrcPath): SpriteEntity(spriteSrcPath)
{
    this->originX = origin.x;
    this->originY = origin.y;
    this->sizeX = spriteSize.x;
    this->sizeY = spriteSize.y;

    srcRect = Rectangle{0, 0, sizeX, sizeY};
    destRect = Rectangle{positionX, positionY, sizeX * scaleX , sizeY * scaleY};
}

SpriteEntity::~SpriteEntity()
{
    UnloadTexture(spriteText);
}

void SpriteEntity::Init()
{
    string pat = string(GetApplicationDirectory()) + spriteSrcPath;
    //std::cout<< pat << endl;
    spriteText = LoadTexture(pat.c_str());
}


void SpriteEntity::Draw()
{
    // Update destination rect

    Vector2 globalPos = this->GetGlobalPosition();
    destRect.x = globalPos.x;
    destRect.y = globalPos.y;
    destRect.width = sizeX * scaleX;
    destRect.height = sizeY * scaleY;

    //DrawTexture(spriteText,positionX,positionY,WHITE);
    DrawTexturePro(spriteText, srcRect, destRect, {originX * scaleX ,originY * scaleY}, rotation * (180.0/PI), WHITE);
}