#include "../header/tilemap.h"
#include <iostream>


TileMap::TileMap()
{
    name = "TileMap" + std::to_string(entityID);
    drawType = TILEMAP;
    drawLayer = baseDrawLayer;
    tileSets = std::map<std::string, Texture2D>();
    tileLayers = new std::vector<tmx::TileLayer>();
}

TileMap::TileMap(std::string name) : TileMap()
{
    this->name = name != "" ? name : "TileMap" + std::to_string(entityID);
}


TileMap::~TileMap()
{
    // Unload all textures
    for(auto tilePair : tileSets)
    {
        TextureManager::instance().UnloadTexture(tilePair.second);  
    }
}


void TileMap::RenderToBufferImage()
{
    // TODO: Render the image to a buffer
}

void TileMap::Draw(float deltaTime)
{
    
    // TODO: Figure out how to draw the tile maps 
    for(tmx::TileLayer layer : *tileLayers)
    //tmx::TileLayer layer = (*tileLayers)[0];
    {
        std::vector<tmx::TileLayer::Tile> tiles = layerTiles[layer.getName()];

        //std::vector<tmx::TileLayer::Tile> tiles = std::vector<tmx::TileLayer::Tile>();
        std::cout << std::to_string(tiles.size()) << std::endl;
        int iterator = 0;

        //Texture2D texture = tileSets[tileSets.begin()->first];
        for(tmx::TileLayer::Tile tile : tiles)
        //tmx::TileLayer::Tile tile = tiles[0];
        {
            // TODO: Optimize the rendering of tilemaps
            int id = tile.ID;
            if (id == 0)
            {
                iterator++;
                continue;
            }

            tmx::Tileset::Tile* t = &mapTiles[id];
            //std::string imagePath = t.imagePath;

            Rectangle srcRect, destRect;
            std::string imagePath = t->imagePath;
            Texture2D* texture = &tileSets[imagePath];
           

            srcRect = (Rectangle){(float)t->imagePosition.x, (float)t->imagePosition.y, (float)t->imageSize.x, (float)t->imageSize.y};
            destRect = (Rectangle){
            0,
            0,
            (float)t->imageSize.x, (float)t->imageSize.y };

            Vector2 center = (Vector2){(30.0f * tileSize.x) /2.0f, (20 * tileSize.y) / 2.0f};
            float xPos = floor(iterator % (int)mapSize.x); 
            float yPos = (iterator/ (int)mapSize.x);

            //std::cout << std::to_string( yPos) << std::endl;
            DrawTexturePro(*texture, srcRect, destRect, (Vector2){(xPos * - tileSize.x) + center.x , (yPos * -tileSize.y) + center.y}, 0, WHITE);
            iterator++;
        }
    }

}

/// @brief Load TMX data for a tilemap using the tmxlite library
/// @param filePath 
void TileMap::LoadDataTMX(std::string filePath)
{
    tmx::Map mapLoader;
    std::string fullPath = (std::string)GetWorkingDirectory() + "/" + filePath;
    mapLoader.load(fullPath.c_str());

    isMapInfinite = mapLoader.isInfinite();
    mapSize = (Vector2){mapLoader.getBounds().width/ mapLoader.getTileSize().x, mapLoader.getBounds().height /mapLoader.getTileSize().y};

    tileSize = (Vector2){(float)mapLoader.getTileSize().x, (float)mapLoader.getTileSize().y};

    const auto& layerData = mapLoader.getLayers();

    for(auto& layer : layerData)
    {
        
        if(layer->getType() == tmx::Layer::Type::Tile)
        {
            tmx::TileLayer tileLayer = (tmx::TileLayer)layer->getLayerAs<tmx::TileLayer>();
            tileLayers->push_back(tileLayer);
            std::vector<tmx::TileLayer::Tile> tiles = tileLayer.getTiles();
            layerTiles.insert({tileLayer.getName(), tiles});
        }
        else if(layer->getType() == tmx::Layer::Type::Object)
        {
            objectLayers->push_back((tmx::ObjectGroup)layer->getLayerAs<tmx::ObjectGroup>()); 
        }
    }
    
    const auto& mapTileSets = mapLoader.getTilesets();

    for(auto set : mapTileSets)
    {
        const std::vector<tmx::Tileset::Tile> tilesInSet = set.getTiles();
        tileSetData.push_back(set);

        // For every unique tile, load any required images
        for(auto tile : tilesInSet)
        {
            if(mapTiles.count(tile.ID + set.getFirstGID()) == 0)
            {
                mapTiles.insert({tile.ID + set.getFirstGID(), tile});
            }

            std::string imagePath = tile.imagePath;
            if(tileSets.count(imagePath) == 0)
            {
                Texture2D image = TextureManager::instance().GetTexture(imagePath);
                tileSets.insert({imagePath, image});
            }
           
        }
    }

    int x = 0;

}