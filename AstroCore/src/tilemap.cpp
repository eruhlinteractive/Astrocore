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


void TileMap::RenderLayer(int layerIndex)
{
    // Render to texture
    BeginTextureMode(frameBuffers[layerIndex]);
    tmx::TileLayer layer = (*tileLayers)[layerIndex];
    std::vector<tmx::TileLayer::Tile> tiles = layerTiles[layer.getName()];
    int iterator = 0;

    for(tmx::TileLayer::Tile tile : tiles)
    {
        int id = tile.ID;
        if (id == 0)
        {
            iterator++;
            continue;
        }
        tmx::Tileset::Tile t = mapTiles[id];
        Rectangle srcRect, destRect;
        std::string imagePath = t.imagePath;
        Texture2D* texture = &tileSets[imagePath];
       
        srcRect = (Rectangle){(float)t.imagePosition.x, (float)t.imagePosition.y, (float)t.imageSize.x, (float)t.imageSize.y};
        destRect = (Rectangle){
        0,
        0,
        (float)t.imageSize.x, (float)t.imageSize.y };
        Vector2 center = (Vector2){(30.0f * tileSize.x) /2.0f, (20 * tileSize.y) / 2.0f};
        float xPos = floor(iterator % (int)mapSize.x); 
        float yPos = (iterator/ (int)mapSize.x);
        DrawTexturePro(*texture, srcRect, destRect, (Vector2){(xPos * - tileSize.x), (yPos * -tileSize.y)}, 0, WHITE);
        iterator++;
    }

    EndTextureMode();

}

void TileMap::RedrawLayer(int layerIndex)
{
    dirtyLayers.push_back(layerIndex);
}

void TileMap::RenderAllLayers()
{
    for(int i = 0; i < tileLayers->size(); i++)
    {
        RenderLayer(i);
    }
}

void TileMap::Draw(float deltaTime)
{
    while(!dirtyLayers.empty())
    {
        int index = dirtyLayers[dirtyLayers.size() - 1];
        RenderLayer(index);
        dirtyLayers.pop_back();
    }

    for(RenderTexture2D layerRender : frameBuffers)
    {
        Rectangle src, dest;
        src = (Rectangle){0,0, (float)layerRender.texture.width, (float)-layerRender.texture.height};
        dest = (Rectangle){0,0, (float)layerRender.texture.width, (float)layerRender.texture.height};
        
        DrawTexturePro(layerRender.texture,src, dest, transform.position, 0, WHITE);
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

            // Initialize frame buffers
            frameBuffers.push_back(LoadRenderTexture(mapSize.x * tileSize.x, mapSize.y * tileSize.y));
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
    RenderAllLayers();
}