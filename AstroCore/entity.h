#include <raylib.h>
#include <vector>
#include <memory>

#ifndef ENTITY2D_H
#define ENTITY2D_H

// TODO: Set up transformation flags
enum TRANSFORM_FLAGS {ROT_UNIQUE, POS_UNIQUE,SCL_UNIQUE};


class Entity2D
{
public:
    // Constructors
    Entity2D();
    Entity2D(Vector2 position, Vector2 scale, float rotation);
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

    /// @brief Get the rotaion of the body in radians
    /// @return The rotation of the body in radians
    float GetRotation() {return rotation;}
    Vector2 GetGlobalPosition();
    float GetRotationDeg(){ return (rotation * 180/PI);}
    Vector2 GetPosition() {return Vector2{positionX,positionY};}
    Vector2 GetScale() {return Vector2{positionX,positionY};}

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

    // Get children
    std::vector<Entity2D*>* GetChildren();
    Entity2D* GetChildAtIndex(int index);
    void AddChild(Entity2D* newChild);
    void RemoveChild(Entity2D* childToCheck);
    bool HasChild(Entity2D* childToCheck);
    int GetChildCount();
    Entity2D* GetParent();

    void SetParent(Entity2D* parent);


protected:

    // The position relative to the parent
    float positionX = 0.0f;
    float positionY = 0.0f;

    float scaleX = 0.0f;
    float scaleY = 0.0f;

    bool isRotationIndependent;

    /// @brief Rotation of this entity in radians
    float rotation = 0.0f;
    bool isReady = false;

    /// @brief The children of this entity
    std::unique_ptr<std::vector<Entity2D*>> children = std::make_unique<std::vector<Entity2D*>>();
    Entity2D* parentEntity = nullptr;
};

#endif // Entity