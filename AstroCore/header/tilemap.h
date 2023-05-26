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
    TileMap(std::string name);
    void LoadDataTMX(std::string filePath);
    ~TileMap();

    void RenderAllLayers();
    void RenderLayer(int layerIndex);

    /// @brief Mark a layer to be re-rendered during the next draw call
    /// @param layerIndex The index of the layer to re-render
    void RedrawLayer(int layerIndex);
    Vector2 GetTileSize();
    void Update(){};
    void Draw(float deltaTime);

protected:

    std::vector<tmx::TileLayer>* tileLayers;
    std::vector<tmx::ObjectGroup>* objectLayers;

    // Vector for the tiles in each layer
    std::map<std::string, std::vector<tmx::TileLayer::Tile>> layerTiles;

    std::vector<RenderTexture2D> frameBuffers;
    std::map<std::string, Texture2D> tileSets;
    std::vector<tmx::Tileset> tileSetData;
    std::map<int, tmx::Tileset::Tile> mapTiles;

    std::vector<int> dirtyLayers;
    
    bool isMapInfinite = false;
    Vector2 mapSize;

    // The draw layer of the lowest 
    int baseDrawLayer = -100;
    Vector2 tileSize;
};
}
#endif // __TILEMAP_H__