#include <raylib.h>
#include <vector>
#include <memory>
#include <string>
#include "structs.h"

#ifndef ENTITY2D_H
#define ENTITY2D_H

// Transformation flags
// Since each flag is stored as a single bit, they need to be shifted to be in the proper positions
// https://dietertack.medium.com/using-bit-flags-in-c-d39ec6e30f08
enum TRANSFORM_FLAGS {
    ROT_UNIQUE = 1 << 0, //1
     POS_UNIQUE = 1 << 1, //2
      SCL_UNIQUE = 1 << 2 //4
      };

// Draw type, defines when the entity is drawn
enum DRAW_TYPE
{
    NORMAL = 1 << 0, // Normal world space drawing
    LIGHT  =1 << 1 // World overlay
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
    
    void SetTransformFlag(TRANSFORM_FLAGS flag) { transformFlags |= (int)flag;};
    void UnsetTransformFlag(TRANSFORM_FLAGS flag) { transformFlags &= ~(int)flag;};
    bool IsTransformFlagSet(TRANSFORM_FLAGS flag){ return (transformFlags & (int)flag) ==(int)flag;};

    Vector2 GetGlobalPosition();
    float GetGlobalRotationDeg();
    float GetGlobalRotation();

    /// @brief Get the path from the scene root to this entity
    /// @return The path to this entity from the scene root
    std::string GetPath();

    int GetId(){return id;}
    // Functions

    /// @brief Called when the object is initialized
    virtual void Init() {};

    /// @brief Called on the first frame the entity is updated
    virtual void Ready(){};

    /// @brief Called each update tick
    /// @param delta The time from the current and the last frame
    virtual void Update(float delta);

    /// @brief Called when the object is de-allocated
    virtual void OnDestroy(){};

    /// @brief  Draw function
    /// @param deltaTime The time between the previous and current frame
    virtual void Draw(float deltaTime);

    /// @brief The layer this sprite is drawn on (lower = first), relative to the parent
    int drawLayer = 0;

    DRAW_TYPE drawType = NORMAL;

    int GetDrawLayer(){
        if(parentEntity != nullptr)
        {
            return drawLayer + parentEntity->GetDrawLayer();
        }
        return drawLayer;
    };

    /// @brief The offset used for Y position-based sorting
    float ySortOffset = 0;

    /// @brief Set the position used for Y sorting
    /// @param newOffset The new offset to use when Y sorting
    void SetYSortOffset(float newOffset){ySortOffset = newOffset;};

    void SetName(std::string newName){this->name = newName;};
    std::string GetName(){return name;};

    // Get children
    std::vector<Entity2D*> GetChildren();
    Entity2D* GetChildAtIndex(int index);
    Entity2D* GetChild(std::string name);
    void AddChild(Entity2D* newChild);
    void RemoveChild(Entity2D* childToCheck);
    bool HasChild(Entity2D* childToCheck);
    int GetChildCount();
    Entity2D* GetParent();

    void SetParent(Entity2D* parent);
    Transform2D transform;

protected:

    bool isReady = false;

    // Bit flags for this entity
    uint8_t transformFlags = 0;
    int id = 0;
    std::string name;

    /// @brief The children of this entity
    std::unique_ptr<std::vector<Entity2D*>> children = std::make_unique<std::vector<Entity2D*>>();
    Entity2D* parentEntity = nullptr;
};
}

#endif // Entity