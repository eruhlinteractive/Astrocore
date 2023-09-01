#include "tilemapLayer.h"
#include <iostream>
#include "../../engine/scene.h"


using namespace Astrolib;

TileMapLayer::TileMapLayer(std::vector<int> *tileIndexes,
                           Texture *tileAtlas,
                           std::map<int, StaticTileMin *> *mapTileInfo,
                           Vector2 mapSize,
                           Vector2 tileSize)
{

    this->tileIndexes = tileIndexes;
    this->tileAtlas = tileAtlas;
    this->mapTileInfo = mapTileInfo;
    this->mapSize = mapSize;
    this->tileSize = tileSize;
}


void TileMapLayer::OnRegister(Scene *scene)
{
    Entity2D::OnRegister(scene);
}



void TileMapLayer::Draw(float deltaTime, Camera2D *camera)
{
    int iterator = 0;

    Rectangle srcRect, destRect;
    int xPos, yPos;
    Vector2 center;
    Texture2D *texture = tileAtlas;
    // std::vector<int> indexes = *tileIndexes;
    int drawn = 0;
    Vector2 screenSize = currentScene->GetWorldRenderSize();
    Vector2 globalPos = GetGlobalPosition();
    SetTextureWrap(*tileAtlas, RL_TEXTURE_WRAP_CLAMP);
    SetTextureFilter(*tileAtlas, TEXTURE_FILTER_POINT);

    
    // Render each tile id
    for (int id : *tileIndexes)
    {
        if (id == 0)
        {
            iterator++;
            continue;
        }

        StaticTileMin *tileInfo = mapTileInfo->at(id);
        if (tileInfo == nullptr)
        {
            iterator++;
            continue;
        }

        xPos = int(iterator % (int)mapSize.x);
        yPos = int(iterator / (int)mapSize.y);

        // AABB test against screen boundaries, and perform "frustum culling'
        Vector2 worldPos = (Vector2){(xPos * tileSize.x) + globalPos.x, (yPos * tileSize.y) + globalPos.y};
        Vector2 screenSpaceCoords = GetWorldToScreen2D(worldPos, * camera);
        Rectangle tileRect = (Rectangle){

            screenSpaceCoords.x,
            screenSpaceCoords.y,
            tileInfo->imageSize.x * camera->zoom,
            tileInfo->imageSize.y * camera->zoom};

        //Vector2 ss = GetScreenToWorld2D({tileRect.x, tileRect.y}, *camera);
        //Rectangle debugRect = (Rectangle){ss.x, ss.y, tileInfo->imageSize.x, tileInfo->imageSize.y};

        
        bool isOnScreen =
            tileRect.x + tileRect.width > 0 &&
            tileRect.x < screenSize.x &&
            tileRect.y + tileRect.height > 0 &&
            tileRect.y < screenSize.y;
            
        //bool isOnScreen = true;
        if (!isOnScreen)
        {
            iterator++;
            continue;
        }

        srcRect = (Rectangle){
            tileInfo->imagePos.x,
            tileInfo->imagePos.y,
            (float)tileInfo->imageSize.x,
            (float)tileInfo->imageSize.y};


        destRect = (Rectangle){
            0,
            0,
            (float)tileInfo->imageSize.x * 1.001f,
            (float)tileInfo->imageSize.y * 1.001f};


        DrawTexturePro(*texture, srcRect, destRect, (Vector2){xPos * -tileSize.x, yPos * -tileSize.y}, 0, WHITE);
        //DrawRectangleLinesEx(debugRect, 1, RED);
        iterator++;
        //drawn++;
    }
    //std::cout << drawn << std::endl;
}
