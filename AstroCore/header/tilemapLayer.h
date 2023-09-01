#ifndef __TILEMAPLAYER_H__
#define __TILEMAPLAYER_H__

using namespace std;
#include "entity.h"
#include <vector>
#include "rlgl.h"
#include <map>
#include "structs.h"

namespace Astrolib
{
    class TileMapLayer : public Entity2D
    {
    public:
        TileMapLayer();
        TileMapLayer(
            std::vector<int>* tileIndexes,
            Texture* tileAtlas,
            std::map<int, StaticTileMin*>* mapTileInfo,
            Vector2 mapSize,
            Vector2 tileSize
        );

        virtual void OnRegister(Scene *scene) override;
        std::vector<int>* tileIndexes;
        Texture* tileAtlas;
        std::map<int, StaticTileMin *>* mapTileInfo;
        Vector2 mapSize;
        Vector2 tileSize;

        void Draw(float deltaTime, Camera2D *camera) override;
    };
    
}

#endif // __TILEMAPLAYER_H__