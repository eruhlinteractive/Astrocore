#include "../header/tilemapLayer.h"
using namespace Astrolib;


TileMapLayer::TileMapLayer(std::vector<int>* tileIndexes,
            Texture* tileAtlas,
            std::map<int, StaticTileMin*>* mapTileInfo,
            Vector2 mapSize,
            Vector2 tileSize)
{

    this->tileIndexes = tileIndexes;
    this->tileAtlas = tileAtlas;
    this->mapTileInfo = mapTileInfo;
    this->mapSize = mapSize;
    this->tileSize = tileSize;
}

void TileMapLayer::Draw(float deltaTime, Vector2 cameraPosition)
{
    int iterator = 0;

    Rectangle srcRect, destRect;
    int xPos, yPos;
    Vector2 center;
    Texture2D* texture = tileAtlas;
    //std::vector<int> indexes = *tileIndexes;

    // Render each tile id
    for (int id : *tileIndexes)
    {
        if (id == 0)
        {
            iterator++;
            continue;
        }
        
        xPos = floor(iterator % (int)mapSize.x); 
        yPos = floor(iterator/ (int)mapSize.x);
        StaticTileMin* tileInfo = mapTileInfo->at(id);

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

        
        DrawTexturePro(*texture, srcRect, destRect, (Vector2){(xPos * - tileSize.x), (yPos * -tileSize.y)}, 0, WHITE);
        iterator++;
    }

}
