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
#include "interfaces.h"
#include <typeinfo>
#include "raylib.h"
#include "cameraEntityBase.h"
#include "pixelPerfectCamera.h"

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

        // Scene(std::string name){};
        Entity2D *FindEntityByName(std::string name);
        Entity2D *GetEntity(std::string path);

        virtual void LoadScene(){};
        virtual void UnloadScene(){};

        virtual void Update(float deltaTime);
        virtual void FixedUpdate(float deltaTime);
        void Draw(float deltaTime);
        bool AddLightToScene(Light2D *light);

        bool RegisterEntity(Entity2D *entity);
        bool UnRegisterEntity(std::string name);

        CameraEntityBase *currentCamera;

        Color ambientColor = WHITE;
        bool ySortEnabled = true;

        /// @brief Sorts two pairs in the map based on their draw index
        /// @param p1 The first pair
        /// @param p2 The second pair
        /// @return Whether the draw layer of the first value is larger than the second
        static bool SortByLayerIndex(Entity2D *p1, Entity2D *p2)
        {
            return p1->GetDrawLayer() < p2->GetDrawLayer();
        }

        static bool SortByLayerIndexWithYSort(Entity2D *p1, Entity2D *p2)
        {

            // Y Position sorting
            if (p1->GetDrawLayer() == p2->GetDrawLayer())
            {
                return p1->transform.position.y < p2->transform.position.y;
            }

            return p1->GetDrawLayer() < p2->GetDrawLayer();
        }

        /// @brief Check if the entity at screenPosition is visible on screen
        /// @param camera The camera that is currently rendering the screen
        /// @param entityRect The rectangle of the entity
        /// @return True if the AABB check passes and the object is visible on screen
        static bool IsOnScreen(Camera2D *camera, Rectangle entityRect)
        {
            Vector2 screenSize = {GetRenderWidth(), GetRenderHeight()};
            Vector2 screenSpaceCoords = GetWorldToScreen2D({entityRect.x, entityRect.y}, *camera);

            Rectangle tileRect = (Rectangle){
                screenSpaceCoords.x,
                screenSpaceCoords.y,
                entityRect.width * camera->zoom,
                entityRect.height * camera->zoom };

            // Vector2 ss = GetScreenToWorld2D({tileRect.x, tileRect.y}, *camera);
            // Rectangle debugRect = (Rectangle){ss.x, ss.y, tileInfo->imageSize.x, tileInfo->imageSize.y};

            bool isOnScreen =
                tileRect.x + tileRect.width > 0 &&
                tileRect.x < screenSize.x &&
                tileRect.y + tileRect.height > 0 &&
                tileRect.y < screenSize.y;

            return isOnScreen;
        }

    private:
        /// @brief Top level of the scene graph
        std::map<std::string, Entity2D *> entities;
        std::map<std::string, Entity2D *> drawableEntities;
        std::map<std::string, Light2D *> lights;

        std::string sceneName = "";
        RenderTexture2D screenSpaceLightMap;
    };
}

#endif // __SCENE_H__