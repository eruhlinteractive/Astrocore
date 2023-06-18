#include "../include/raylib.h"
#include <vector>
#include <memory>
#include <string>
#include "structs.h"

#ifndef __ENTITY2D_H__
#define __ENTITY2D_H__

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

// Draw type, defines when the entity is drawn
enum ENTITY_TYPE
{
    BASE,
    SPRITE,
    TILEMAP,
    LIGHT
};

namespace Astrolib
{
    class Entity2D
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

        Vector2 GetGlobalPosition();
        float GetGlobalRotationDeg();
        float GetGlobalRotation();

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

        virtual void Draw(float deltaTime, Vector2 cameraPosition) = 0;

        /// @brief The offset used for Y position-based sorting
        float ySortOffset = 0;

        /// @brief Set the position used for Y sorting
        /// @param newOffset The new offset to use when Y sorting
        void SetYSortOffset(float newOffset) { ySortOffset = newOffset; };

        void SetName(std::string newName) { this->name = newName; };
        std::string GetName() { return name; };

        // Get children
        std::vector<Entity2D *> GetChildren();
        Entity2D *GetChildAtIndex(int index);
        Entity2D *GetChild(std::string name);
        void AddChild(Entity2D *newChild);
        void RemoveChild(Entity2D *childToCheck);
        bool HasChild(Entity2D *childToCheck);
        int GetChildCount();
        Entity2D *GetParent();

        void SetParent(Entity2D *parent);
        Transform2D transform;

    protected:
        ENTITY_TYPE type;
        bool isReady = false;

        // Bit flags for this entity
        uint8_t transformFlags = 0;
        int entityID = 0;
        std::string name;

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