#include <raylib.h>
#include <vector>
#include <memory>
#include <string>

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

namespace Astrolib
{
class Entity2D
{
public:
    // Constructors

    Entity2D();
    Transform transform;
    Entity2D(std::string name);
    Entity2D(std::string name, Vector2 position, Vector2 scale, float rotation);
    virtual ~Entity2D();

    // Transformation functions

    void MoveLocal(Vector2 movement);
    void MoveGlobal(Vector2 movement);

    void Rotate(float rotRad);
    void RotateAroundPoint(float rotRad, Vector2 point);

    void RotateDegAroundPoint(float rotDeg, Vector2 point);
    void RotateDeg(float rotDeg);
   

    void SetPosition(Vector2 newPosition);
    void Scale(Vector2 scaleDelta);
    void SetScale(Vector2 newScale);
    void SetRotation(float newRotation);
    void SetRotationDeg(float newRotationDeg);
    void ScaleTowardsPoint(Vector2 point, Vector2 scaleDelta);

    // Flag operations 
    
    void SetTransformFlag(TRANSFORM_FLAGS flag) { transformFlags |= (int)flag;};
    void UnsetTransformFlag(TRANSFORM_FLAGS flag) { transformFlags &= ~(int)flag;};
    bool IsFlagSet(TRANSFORM_FLAGS flag){ return (transformFlags & (int)flag) ==(int)flag;};

    /// @brief Get the rotaion of the body in radians
    /// @return The rotation of the body in radians
    float GetRotation() {return rotation;}
    Vector2 GetGlobalPosition();
    float GetRotationDeg(){ return (rotation * 180/PI);}
    float GetGlobalRotationDeg();
    float GetGlobalRotation();
    Vector2 GetPosition() {return position;}
    Vector2 GetScale() {return scale;}

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

    void SetName(std::string newName){name = newName;};
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


protected:

    // The position relative to the parent
    Vector2 position;
    Vector2 scale;

    /// @brief Rotation of this entity in radians
    float rotation = 0.0f;
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