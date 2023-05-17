#ifndef __SCENE_H__
#define __SCENE_H__

#include "entity.h"
#include <map>
#include <string>
#include <algorithm>

namespace Astrolib
{
class Scene
{

public:
    Scene();
    Scene(std::string name);
    Entity2D* FindEntityByName(std::string name);

    void Update(float deltaTime);
    void Draw(float deltaTime);

    bool RegisterEntity(std::string name, Entity2D* entity);
    bool UnRegisterEntity(std::string name);

    /// @brief Sorts two pairs in the map based on their draw index
    /// @param p1 The first pair
    /// @param p2 The second pair
    /// @return Whether the draw layer of the first value is larger than the second
    static bool SortByLayerIndex(Entity2D* p1, Entity2D* p2){

        // Y Position sorting
        if(p1->drawLayer == p2->drawLayer)
        {
            return p1->GetPosition().y < p2->GetPosition().y;
        }
        
        return p1->drawLayer < p2->drawLayer;
    }

private:
    std::map<std::string, Entity2D*> entities;
};
}

#endif // __SCENE_H__