#ifndef __TILEMAP_H__
#define __TILEMAP_H__

#include "../header/entity.h"

namespace Astrolib
{
class TileMap : public Entity2D
{
public:
    TileMap();
    TileMap(int tileSizeX, int tileSizeY, std::string name);
    void LoadDataTMX(std::string filePath);

    Vector2 GetTileSize();
    void Draw(float deltaTime);

protected:
    void* tileMapData;
    // The draw layer of the lowest 
    int baseDrawLayer = -100;
    int tileSizeX;
    int tileSizeY;
};
}
#endif // __TILEMAP_H__