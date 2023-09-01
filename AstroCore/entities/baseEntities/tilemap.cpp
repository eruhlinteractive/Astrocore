#include "tilemap.h"
#include <iostream>

using namespace Astrolib;

TileMap::TileMap()
{
    type = TILEMAP;
    name = "TileMap" + std::to_string(entityID);
    drawLayer = baseDrawLayer;
    tileSets = std::map<std::string, Texture2D>();
    staticMapTiles = new std::map<int, StaticTileMin*>();
    //tileLayers = new std::vector<tmx::TileLayer>();
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
    // Unload render texture
    UnloadRenderTexture(mapTextureAtlas);
}

void TileMap::RenderLayer(int layerIndex)
{
    // TODO: Cull tile rendering based on what's on screen
    int iterator = 0;

    Texture2D texture = mapTextureAtlas.texture;
    Rectangle srcRect, destRect;
    int xPos, yPos;
    Vector2 center;
    TileMapLayer* layer = tileLayers[layerIndex];
    std::vector<int> indexes = *(layer->tileIndexes);
    
    // Render each tile id
    for (int id : indexes)
    {
        if (id == 0)
        {
            iterator++;
            continue;
        }
        
        xPos = floor(iterator % (int)mapSize.x); 
        yPos = floor(iterator/ (int)mapSize.x);

        StaticTileMin* tileInfo = staticMapTiles->at(id);

        if(tileInfo == nullptr)
        {
            continue;
        }

        srcRect = (Rectangle){
            tileInfo->imagePos.x,
            tileInfo->imagePos.y,
            (float)tileInfo->imageSize.x,
            (float)tileInfo->imageSize.y
        };

        destRect = (Rectangle){
            0,
            0,
            (float)tileInfo->imageSize.x,
            (float)tileInfo->imageSize.y};

        
        DrawTexturePro(texture, srcRect, destRect, (Vector2){(xPos * - tileSize.x), (yPos * -tileSize.y)}, 0, WHITE);
        iterator++;
    }
    
}


void TileMap::OnRegister(Scene* scene)
{
    // Register base before also registering layers
    Entity2D::OnRegister(scene);
    for(auto layer : tileLayers)
    {
        layer->OnRegister(scene);
    }

}


void TileMap::SetupLayers()
{
    int iterator = 0;
    for(auto layerIndexes : layers)
    {
        TileMapLayer* newLayer = new TileMapLayer(
            layerIndexes.second,
            &(mapTextureAtlas.texture),
            staticMapTiles,
            mapSize,
            tileSize
        );

        tileLayers.push_back(newLayer);
    }

}

void TileMap::RenderAllLayers()
{
    for(int i = 0; i < layers.size(); i++)
    {
        RenderLayer(i);
    }
}


void TileMap::SetDrawLayerForMapLayer(int layerIndex, int newLayerDrawIndex)
{
    assert(layerIndex < tileLayers.size() && " Layer index is out of bounds");
    TileMapLayer* layer = tileLayers[layerIndex];
    layer->drawLayer = newLayerDrawIndex;
}


void TileMap::Draw(float deltaTime, Camera2D* camera)
{
    Vector2 globalPos = GetGlobalPosition();
    int startXIndex;
    int endXIndex;
    int startYIndex;
    int endYIndex;
    
    RenderAllLayers();
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

    int iterator = 0;
    for(auto& layer : layerData)
    {
        
        if(layer->getType() == tmx::Layer::Type::Tile)
        {
            tmx::TileLayer tileLayer = (tmx::TileLayer)layer->getLayerAs<tmx::TileLayer>();
            //tileLayers->push_back(tileLayer);
            std::vector<tmx::TileLayer::Tile> tiles = tileLayer.getTiles();
            layers[iterator] = new std::vector<int>();

            for(auto t : tiles)
            {
                (layers[iterator])->push_back(t.ID);
            }
            iterator++;
        }
        //else if(layer->getType() == tmx::Layer::Type::Object)
        //{
        //    objectLayers->push_back((tmx::ObjectGroup)layer->getLayerAs<tmx::ObjectGroup>()); 
        //}
    }
    
    const auto& mapTileSets = mapLoader.getTilesets();

    int maxTileWidth = 0;
    int maxTileHeight = 0;

    // Populate array of tiles
    for(auto set : mapTileSets)
    {
        const std::vector<tmx::Tileset::Tile> tilesInSet = set.getTiles();
        //tileSetData.push_back(set);

        // For every unique tile, load any required images and push to global dictionary
        for(auto tile : tilesInSet)
        {
            if(mapTiles.count(tile.ID + set.getFirstGID()) == 0)
            {
                mapTiles.insert({tile.ID + set.getFirstGID(), tile});
            }

            maxTileWidth = tile.imageSize.x > maxTileWidth ? tile.imageSize.x : maxTileWidth;
            maxTileHeight = tile.imageSize.y > maxTileHeight ? tile.imageSize.y : maxTileHeight;

            std::string imagePath = tile.imagePath;
            if(tileSets.count(imagePath) == 0)
            {
               Texture2D image = TextureManager::instance().GetTexture(imagePath);
               tileSets.insert({imagePath, image});
            }
           
        }
    }

    // Render atlas texture
    
    
    // Create horizontal render texture to act as a tile atlas
    
    mapTextureAtlas = LoadRenderTexture(maxTileWidth * mapTiles.size() + 3 * mapTiles.size(), maxTileHeight);
    BeginDrawing();

    BeginTextureMode(mapTextureAtlas);
    int newX = 0;
    int newY = 0;

    for (std::pair p : mapTiles)
    {
    
        tmx::Tileset::Tile tile = p.second;
        //std::cout << i << endl;
        if(staticMapTiles->count(p.first) > 0)
        {
           continue;
        }

        std::string path = tile.imagePath;
        //std::cout << path << endl;
        Texture2D image = TextureManager::instance().GetTexture(path);

        if(path != "")
        {
            // TODO: Fix the rendering of these tiles

            Rectangle source = (Rectangle){
                (float)tile.imagePosition.x , 
                (float)tile.imagePosition.y, 
                (float)tile.imageSize.x, 
                (float)tile.imageSize.y * -1.0f
            };
            
            Rectangle dest = (Rectangle){(float)newX, (float)newY, (float)tile.imageSize.x, (float)tile.imageSize.y};
            // Render to atlas
            //DrawTexture(image, 0, 0, WHITE);
            //DrawTexturePro(image, source, dest, {0, 0} , 0 , WHITE);
            DrawTextureRec(image, source, (Vector2){(float)newX, maxTileHeight -  (float)tile.imageSize.y}, WHITE);
        }
    
        //std::cout << path << std::endl;

        
        StaticTileMin* newTile = new StaticTileMin( 
            (Vector2){(float)tile.imageSize.x, float(tile.imageSize.y)},
            (Vector2){(float)newX , (float)newY}
        );

        // Store updated coordinates
        
        staticMapTiles->insert({p.first, newTile});
        newX += tile.imageSize.x + 3;
    }
    EndTextureMode();
    EndDrawing();

    // Disable wrapping to prevent artifacts
    // https://gamedev.stackexchange.com/questions/74420/seamless-tilemap-rendering-borderless-adjacent-images
    SetTextureWrap(mapTextureAtlas.texture, TEXTURE_WRAP_CLAMP);
    SetTextureFilter(mapTextureAtlas.texture, TEXTURE_FILTER_POINT);
    /*
    for(std::pair<std::string, Texture2D> tile_pair : tileSets)
    {
        std::string path = tile_pair.first;
        TextureManager::instance().UnloadTexture(path);
        
    }
    */

    //RenderAllLayers();
    SetupLayers();
}