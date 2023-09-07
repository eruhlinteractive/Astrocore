#ifndef __SCENE_H__
#define __SCENE_H__

#include "../entities/baseEntities/entity.h"
#include "../entities/baseEntities/stackedSpriteEntity.h"
#include "texturemanager.h"
#include "../entities/baseEntities/light2D.h"
#include "rlgl.h"
#include <map>
#include <string>
#include <algorithm>
#include <regex>
#include "../entities/baseEntities/tilemap.h"
#include "../data/interfaces.h"
#include <typeinfo>
#include "raylib.h"
#include "../entities/baseEntities/cameraEntityBase.h"
#include "../entities/baseEntities/pixelPerfectCamera.h"
#include "../entities/baseEntities/cameraEntity.h"
#include "../entities/baseEntities/spriteEntity.h"
#include "collisionTracker.h"
#include <box2d/box2d.h>

// Custom Blend Modes
#define RLGL_SRC_ALPHA 0x0302
#define RLGL_MIN 0x8007
#define RLGL_MAX 0x8008

namespace Astrolib
{
    class Scene : public Observer, public Signaler
    {
    public:
        Scene();
        ~Scene();

        // Scene(std::string name){};
        Entity2D *FindEntityByName(std::string name);
        Entity2D *GetEntity(std::string path);

        virtual void LoadScene(){
            isSceneLoaded = true;
        };
        virtual void UnloadScene(){};

        virtual void Update(float deltaTime);
        virtual void FixedUpdate(float deltaTime);
        virtual void LateUpdate(float deltaTime);
        virtual void Draw(float deltaTime);

        bool AddLightToScene(Light2D *light);

        void OnNotify(const Signaler *signaler, std::string eventName) override;

        bool RegisterEntity(Entity2D *entity);
        bool UnRegisterEntity(std::string name);

        CameraEntityBase *currentCamera = nullptr;

        Color ambientColor = WHITE;
        bool ySortEnabled = true;

        Entity2D *GetRoot()
        {
            return root;
        }

        /// @brief Sorts two pairs in the map based on their draw index
        /// @param p1 The first pair
        /// @param p2 The second pair
        /// @return Whether the draw layer of the first value is larger than the second
        inline static bool SortByLayerIndex(Entity2D *p1, Entity2D *p2)
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
        bool IsOnScreen(Rectangle entityRect)
        {
            Vector2 renderSize = currentCamera->GetRenderResolution();
            Vector2 screenSpaceCoords = GetWorldToScreen2D({entityRect.x, entityRect.y}, *(currentCamera->GetCamera()));
            Rectangle tileRect = (Rectangle){
                screenSpaceCoords.x,
                screenSpaceCoords.y,
                entityRect.width,
                entityRect.height};

            //Debug::Log(std::to_string(tileRect.x));
            // Vector2 ss = GetScreenToWorld2D({tileRect.x, tileRect.y}, *camera);
            // Rectangle debugRect = (Rectangle){ss.x, ss.y, tileInfo->imageSize.x, tileInfo->imageSize.y};

            bool isOnScreen =
                tileRect.x + tileRect.width > 0 &&
                tileRect.x < renderSize.x &&
                tileRect.y + tileRect.height > 0 &&
                tileRect.y < renderSize.y;

            return isOnScreen;
        }

        Vector2 GetWorldRenderSize();

        b2World *GetPhysicsWorld()
        {
            return physicsWorld;
        }

        /// @brief Register a resource with a given name (service locator pattern)
        /// @param name The name of the resource to register
        /// @param resource A pointer to the resource to register
        void RegisterResource(string name, void* resource);

        /// @brief Get a resource with a given name
        /// @param name The name of the resource to get
        /// @return A void pointer to the resource, or nullptr if the resource hasn't been registered yet
        void* GetResource(string name);
        bool HasResourve(string name);

    protected:
        b2World *physicsWorld;
        // Is the processing currently paused?
        bool isPaused = false;
        // Has the scene already been loaded
        bool isSceneLoaded = false;


    private:
        Entity2D *root;
        
        /// @brief Top level of the scene graph
        std::map<std::string, Entity2D *> entities;
        std::map<int, Entity2D *> entityIDMap;
        std::map<std::string, Entity2D *> drawableEntities;
        std::map<std::string, Light2D *> lights;

        std::string sceneName = "";
        RenderTexture2D screenSpaceLightMap;
        CollisionTracker colTracker;
    };
}

#endif // __SCENE_H__