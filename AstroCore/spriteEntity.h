#include <raylib.h>
#include "entity.h"
#include <string>

#ifndef Entity2D_H
#define Entity2D_H
#endif

using namespace std;

class SpriteEntity: public Entity2D
{

public:
    SpriteEntity(string spriteSrcPath);
    SpriteEntity(Vector2 origin, Vector2 spriteSize, string spriteSrcPath);
    void Init();
    virtual void Draw();
    ~SpriteEntity();


private:
    string spriteSrcPath;
    Texture2D spriteText;
    float originX = 0;
    float originY = 0;
    float sizeX = 0;
    float sizeY = 0;


    Rectangle srcRect;
    Rectangle destRect;
    
};