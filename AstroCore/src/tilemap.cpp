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

TileMap::TileMap(int tileSizeX, int tileSizeY, std::string name) : TileMap()
{
    this->tileSizeX = tileSizeX;
    this->tileSizeY = tileSizeY;
    this->name = name != "" ? name : "TileMap" + std::to_string(entityID);
}

void TileMap::Draw(float deltaTime)
{
    // TODO: Figure out how to draw the tile maps 
    for(tmx::TileLayer layer : *tileLayers)
    {
        std::vector<tmx::TileLayer::Tile> tiles = layer.getTiles();
        int iterator = 0;

        for(tmx::TileLayer::Tile tile : tiles)
        {
            int id = tile.ID;
            auto t = tileSetData[0].getTile(id);
            //std::cout << std::to_string( int(iterator / 20)) << std::endl;
            if(t == nullptr)
            {
                iterator++;
                continue;
            }

            Rectangle srcRect, destRect;
            Texture2D texture = tileSets[(t->imagePath)];

            srcRect = (Rectangle){(float)t->imagePosition.x, (float)t->imagePosition.y, (float)t->imageSize.x, (float)t->imageSize.y};
            destRect = (Rectangle){
            0,
            0,
            16,16 };

            Vector2 center = (Vector2){(30.0f * 16.0f) /2.0f, 20 * 16/2.0};
            //DrawTexturePro(texture, srcRect, destRect, (Vector2){
            //    (transform.position.x - center.x) + iterator * 16.0f,(transform.position.y - center.y)},0,WHITE);
            
            float xPos = int(floor(iterator % 30)); 
            float yPos = (iterator/ 30);

            //std::cout << std::to_string( yPos) << std::endl;
            DrawTexturePro(texture, srcRect, destRect, (Vector2){(xPos * -16) + center.x , (yPos * -16) + center.y}, 0, WHITE);
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

    const auto& layerData = mapLoader.getLayers();

    for(auto& layer : layerData)
    {
        tmx::TileLayer t = (*layer.get()).getLayerAs<tmx::TileLayer>();
        if(layer->getType() == tmx::Layer::Type::Tile)
        {
            tmx::TileLayer tileLayer = (tmx::TileLayer)layer->getLayerAs<tmx::TileLayer>();
            tileLayers->push_back(tileLayer);
            tmx::TileLayer l = *(tileLayers->begin());
        }
    }
    const auto& mapTileSets = mapLoader.getTilesets();

    for(auto set : mapTileSets)
    {
        tileSetData.push_back(set);
        std::string imagePath = set.getImagePath();
        Texture2D image = TextureManager::instance().GetTexture(imagePath);
        tileSets.insert({imagePath, image});
        
    }

}