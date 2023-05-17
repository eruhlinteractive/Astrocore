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
    ~Scene();


    //Scene(std::string name){};
    Entity2D* FindEntityByName(std::string name);

    virtual void LoadScene(){};
    virtual void UnloadScene(){};

    virtual void Update(float deltaTime);
    void Draw(float deltaTime);

    bool RegisterEntity(std::string name, Entity2D* entity);
    bool UnRegisterEntity(std::string name);

    Camera2D* currentCamera;

    bool ySortEnabled = true;

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

    static bool SortByLayerIndexWithYSort(Entity2D* p1, Entity2D* p2){

        // Y Position sorting
        if(p1->drawLayer == p2->drawLayer)
        {
            return p1->GetPosition().y < p2->GetPosition().y;
        }

        return p1->drawLayer < p2->drawLayer;
    }

    /// @brief Check if the entity at screenPosition is visible on screen
    /// @param screenPosition The position of the object on the screen
    /// @param buffer The padding size used to prevent pop-in near the edge of the screen
    /// @return True if the AABB check passes and the object is visible on screen
    static bool IsOnScreen(Vector2 screenPosition, float buffer = 0.2)
    {
        Vector2 size = (Vector2){(float)GetScreenWidth(), (float)GetScreenHeight()};
        return (screenPosition.x - buffer <= size.x && screenPosition.x + buffer >= 0.0) &&
        (screenPosition.y - buffer <= size.y && screenPosition.y + buffer >= 0.0);
    }

private:
    std::map<std::string, Entity2D*> entities;
    std::string sceneName = "";
};
}

#endif // __SCENE_H__