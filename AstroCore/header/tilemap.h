#ifndef __TILEMAP_H__
#define __TILEMAP_H__

#include "../header/entity.h"
#include "../header/filemanager.h"
#include "../header/texturemanager.h"

#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>

using namespace Astrolib;

namespace Astrolib
{
class TileMap : public Entity2D
{
public:
    TileMap();
    TileMap(int tileSizeX, int tileSizeY, std::string name);
    void LoadDataTMX(std::string filePath);
    ~TileMap(){};

    Vector2 GetTileSize();
    void Update(){};
    void Draw(float deltaTime);

protected:

    std::vector<tmx::TileLayer>* tileLayers;

    std::map<std::string, Texture2D> tileSets;
    std::vector<tmx::Tileset> tileSetData;
    
    // The draw layer of the lowest 
    int baseDrawLayer = -100;
    int tileSizeX;
    int tileSizeY;
};
}
#endif // __TILEMAP_H__