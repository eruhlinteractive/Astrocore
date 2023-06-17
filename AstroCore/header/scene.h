#ifndef __SCENE_H__
#define __SCENE_H__

#include "entity.h"
#include "texturemanager.h"
#include "light2D.h"
#include "rlgl.h"
#include <map>
#include <string>
#include <algorithm>
#include <regex>
#include "tilemap.h"

// Custom Blend Modes
#define RLGL_SRC_ALPHA 0x0302
#define RLGL_MIN 0x8007
#define RLGL_MAX 0x8008

namespace Astrolib
{
class Scene
{

public:
    Scene();
    ~Scene();


    //Scene(std::string name){};
    Entity2D* FindEntityByName(std::string name);
    Entity2D* GetEntity(std::string path);

    virtual void LoadScene(){};
    virtual void UnloadScene(){};

    virtual void Update(float deltaTime);
    void Draw(float deltaTime);
    bool AddLightToScene(Light2D* light);

    bool RegisterEntity(Entity2D* entity);
    bool UnRegisterEntity(std::string name);

    Camera2D* currentCamera;

    Color ambientColor = WHITE;
    bool ySortEnabled = true;

    /// @brief Sorts two pairs in the map based on their draw index
    /// @param p1 The first pair
    /// @param p2 The second pair
    /// @return Whether the draw layer of the first value is larger than the second
    static bool SortByLayerIndex(Entity2D* p1, Entity2D* p2){
        return p1->GetDrawLayer() < p2->GetDrawLayer();
    }

    static bool SortByLayerIndexWithYSort(Entity2D* p1, Entity2D* p2){

        // Y Position sorting
        if(p1->GetDrawLayer() == p2->GetDrawLayer())
        {
            return p1->transform.position.y < p2->transform.position.y;
        }

        return p1->GetDrawLayer() < p2->GetDrawLayer();
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
    /// @brief Top level of the scene graph
    std::map<std::string, Entity2D*> entities;
    std::map<std::string, Light2D*> lights;

    std::string sceneName = "";
    RenderTexture2D screenSpaceLightMap;
};
}

#endif // __SCENE_H__