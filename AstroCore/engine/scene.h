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
#include <raylib.h>
#include "entityScene.h"
#include "../external/json.hpp"


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
    class Scene : public Observer, public Signaler, public EntityScene
    {
    friend class Game;

    public:
        Scene();
        ~Scene();

        // Scene(std::string name){};
        virtual bool IsOnScreen(Rectangle entityRect) override final;
        /*
         
        */
        virtual void LoadScene()
        {
            isSceneLoaded = true;
        };

        virtual void UnloadScene(){};

        virtual void Update(float deltaTime);
        virtual void FixedUpdate(float deltaTime);
        virtual void LateUpdate(float deltaTime);
        virtual void Draw(float deltaTime);

        bool AddLightToScene(Light2D *light);

        void OnNotify(const Signaler *signaler, std::string eventName) override;
        
        /// @brief Get the fraction of how far we are to the next physics step
        /// @return The fraction (0,1) of how far we are towards the next physics step
        virtual float GetPhysicsFraction() override final;
        
        //Entity management/access

        /// @brief Register an entity to the scene
        /// @param entity The entity to register
        /// @return True if the entity was registered successfully
        bool RegisterEntity(Entity2D *entity);

        /// @brief Unregisters an entity from the scene
        /// @param name The name of the entity to unregister
        /// @return True if the entity was successfully unregistered
        bool UnRegisterEntity(std::string name);
        virtual Entity2D* GetEntity(std::string path) override final;
        virtual Entity2D *FindEntityByName(std::string name) override final;


        virtual void RegisterResource(string name, void* resource) override final;
        virtual void* GetResource(string name) override final;
        virtual bool HasResource(string name) override final;
        

        Color ambientColor = WHITE;
        bool ySortEnabled = true;

        Entity2D *GetRoot()
        {
            return root;
        }

        virtual CameraEntityBase* GetCurrentCamera() override final;
        virtual void SetCurrentCamera(CameraEntityBase* newCamera) override final;

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

        virtual Vector2 GetWorldRenderSize() override final;

        virtual b2World *GetPhysicsWorld() override final
        {
            return physicsWorld;
        }

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
        std::map<std::string, void*> sceneResources;

        std::string sceneName = "";
        CameraEntityBase *currentCamera = nullptr;
        RenderTexture2D screenSpaceLightMap;
        CollisionTracker colTracker;

        // Only accessible directly by friends :)
        float physicsStepFraction = 0;
    };
}

#endif // __SCENE_H__