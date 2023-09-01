#ifndef __TILEMAP_H__
#define __TILEMAP_H__

#include "entity.h"
#include "../../engine/filemanager.h"
#include "../../engine/texturemanager.h"
#include <tmxlite/Config.hpp>
#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>
#include "../../data/structs.h"
#include "tilemapLayer.h"

namespace Astrolib
{
    class TileMap : public Entity2D
    {
    public:
        TileMap();
        TileMap(std::string name);
        void LoadDataTMX(std::string filePath);
        ~TileMap();

        // Game::GetCurrentScene();
        void RenderAllLayers();
        void RenderLayer(int layerIndex);

        virtual void OnRegister(Scene* scene) override;

        void SetupLayers();        
        void SetDrawLayerForMapLayer(int layerIndex, int newSortIndex);
        Vector2 GetTileSize();
        void Update(){};
        void Draw(float deltaTime, Camera2D *camera) override;
        std::vector<TileMapLayer*> GetTileLayers(){ return tileLayers;};

    protected:
        std::vector<TileMapLayer*> tileLayers;
        std::map<int, std::vector<int>*> layers;

        // Vector for the tiles in each layer
        std::map<std::string, std::vector<tmx::TileLayer::Tile>> layerTiles;

        std::map<std::string, Texture2D> tileSets;

        RenderTexture2D mapTextureAtlas;

        std::map<int, tmx::Tileset::Tile> mapTiles;

        std::map<int, StaticTileMin *>* staticMapTiles;

        // TODO: Add support for infinite tilemaps
        bool isMapInfinite = false;
        Vector2 mapSize;

        // The draw layer of the lowest
        int baseDrawLayer = -100;
        Vector2 tileSize;
    };
}
#endif // __TILEMAP_H__