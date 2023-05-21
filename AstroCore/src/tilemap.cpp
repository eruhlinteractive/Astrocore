#include "../header/tilemap.h"


using namespace Astrolib;


TileMap::TileMap()
{
    name = "TileMap" + std::to_string(entityID);
    drawType = TILEMAP;
    drawLayer = baseDrawLayer;
}

TileMap::TileMap(int tileSizeX, int tileSizeY, std::string name)
{
    this->tileSizeX = tileSizeX;
    this->tileSizeY = tileSizeY;
    this->name = name != "" ? name : "TileMap" + std::to_string(entityID);
}

void TileMap::LoadDataTMX(std::string filePath)
{
    // TODO: Load data from file manager 
}