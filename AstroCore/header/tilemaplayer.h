#ifndef __TILEMAPLAYER_H__
#define __TILEMAPLAYER_H__

using namespace std;
#include "entity.h"
#include <vector>

namespace Astrolib
{
    class TileMapLayer : public Entity2D
    {
        std::vector<int> tileIndexes;
        Texture *tileAtlas;

        void Draw(float deltaTime, Vector2 cameraPosition)
        {
            // TODO
        }
    };
}

#endif // __TILEMAPLAYER_H__