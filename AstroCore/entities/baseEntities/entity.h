
#ifndef __ENTITY2D_H__
#define __ENTITY2D_H__

#include <raylib.h>
#include <vector>
#include <memory>
#include <string>
#include "../../data/structs.h"
#include "../../data/interfaces.h"
#include "../../engine/debug.h"
#include "../../engine/input.h"

/*
    Base class for every entity in the scene (except a few special cases like lights)
    Provides basic signatures for updating and rendering the entity

*/

// Transformation flags
// Since each flag is stored as a single bit, they need to be shifted to be in the proper positions
// https://dietertack.medium.com/using-bit-flags-in-c-d39ec6e30f08
enum TRANSFORM_FLAGS
{
    ROT_UNIQUE = 1 << 0, // 1
    POS_UNIQUE = 1 << 1, // 2
    SCL_UNIQUE = 1 << 2  // 4
};

// Draw type, defines when/how the entity is drawn
enum ENTITY_TYPE
{
    BASE,
    SPRITE,
    TILEMAP,
    LIGHT,
    CAMERA,
    PIXELCAMERA,
    PHYSICAL
};

namespace Astrolib
{
    class Scene;
    class Entity2D : public Observer, public Signaler
    {
    public:
        // Constructors
        Entity2D();
        Entity2D(std::string name);
        Entity2D(std::string name, Vector2 position, Vector2 scale, float rotation);
        virtual ~Entity2D();
        // Flag operations

        

        void SetTransformFlag(TRANSFORM_FLAGS flag) { transformFlags |= (int)flag; };
        void UnsetTransformFlag(TRANSFORM_FLAGS flag) { transformFlags &= ~(int)flag; };
        bool IsTransformFlagSet(TRANSFORM_FLAGS flag) { return (transformFlags & (int)flag) == (int)flag; };

        virtual Vector2 GetGlobalPosition();
        virtual float GetGlobalRotationDeg();
        virtual float GetGlobalRotation();

        /// @brief Called when a entity is registered in a scene
        /// @param scene The scene the entity was registered in
        virtual void OnRegister(Scene *scene);

        /// @brief Get the path from the scene root to this entity
        /// @return The path to this entity from the scene root
        std::string GetPath();

        int GetEntityID() { return entityID; }
        // Functions

        ENTITY_TYPE GetType() { return type; }

        /// @brief Called when the object is initialized
        virtual void Init(){};

        /// @brief Called on the first frame the entity is updated
        virtual void Ready(){};

        /// @brief Called each update tick
        /// @param delta The time from the current and the last frame
        virtual void Update(float delta);

        /// @brief Called after every entity has had Update() called on them
        /// @param delta Time elapsed since the last frame
        virtual void LateUpdate(float delta){};

        virtual void FixedUpdate(float delta){};

        /// @brief Called when the object is de-allocated
        virtual void OnDestroy(){};

        /// @brief The layer this sprite is drawn on (lower = first), relative to the parent
        int drawLayer = 0;
        int GetDrawLayer()
        {
            if (parentEntity != nullptr)
            {
                return drawLayer + parentEntity->GetDrawLayer();
            }
            return drawLayer;
        };

        /// @brief Get the sprite rectangle, or a zero-sized rectangle if no sprite is set
        /// @return The Rectangle bounding box of the sprite
        virtual Rectangle GetBoundRect()
        {
            Vector2 globalPos = GetGlobalPosition();
            return (Rectangle){globalPos.x, globalPos.y, 0, 0};
        };

        virtual void Draw(float deltaTime, Camera2D *camera){};

        /// @brief The offset used for Y position-based sorting
        float ySortOffset = 0;

        /// @brief Set the position used for Y sorting
        /// @param newOffset The new offset to use when Y sorting
        void SetYSortOffset(float newOffset) { ySortOffset = newOffset; };

        void SetName(std::string newName) { this->name = newName; };
        std::string GetName() { return name; };

        ///@brief Get a list of all children of this entity
        std::vector<Entity2D *> GetChildren();
        Entity2D *GetChildAtIndex(int index);
        Entity2D *GetChild(std::string name);
        void AddChild(Entity2D *newChild);
        void RemoveChild(Entity2D *childToCheck);
        bool HasChild(Entity2D *childToCheck);
        int GetChildCount();
        Entity2D *GetParent();

        Scene *GetCurrentScene();
        void SetParent(Entity2D *parent);
        Transform2D transform;
        inline static float pixelsPerUnit = 1.0f;
        inline static Vector2 worldDrawSize = {0, 0};

        /// @brief Set the visibility of this entity, applying the value to all children
        /// @param isVisible Is the entity visible?
        void SetVisible(bool isVisible);

        /// @brief Is this node currently visible?
        /// @return True if the node is visible
        inline bool IsVisible() { return isVisible;}
        

        float GetNearestMultiple(float value, float multiple)
        {
            if (multiple == 0)
            {
                return value;
            }

            float rem = fmodf(value, multiple);

            if (rem == 0)
            {
                return value;
            }
            float result = value - rem;

            if (rem > (multiple / 2.0f))
                result += multiple;

            return result;
        }

    protected:
        bool isVisible = true;
        ENTITY_TYPE type;
        bool isReady = false;
        Scene *currentScene = nullptr;

        // Bit flags for this entity
        uint8_t transformFlags = 0;
        int entityID = 0;
        std::string name ;

        /// @brief The children of this entity
        std::unique_ptr<std::vector<Entity2D *>> children = std::make_unique<std::vector<Entity2D *>>();
        Entity2D *parentEntity = nullptr;

    private:
        // Count of all entities in a scene
        // Used to assign a unique ID to each entity
        static int entityCount;
    };
}

#endif // __ENTITY2D_H__