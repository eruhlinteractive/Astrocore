#include <raylib.h>
#include <vector>
#include <memory>

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
    void RotateDeg(float rotDeg);

    void SetPosition(Vector2 newPosition);
    void SetScale(Vector2 newScale);
    void SetRotation(float newRotation);
    void SetRotationDeg(float newRotationDeg);

    /// @brief Get the rotaion of the body in radians
    /// @return The rotation of the body in radians
    float GetRotation() {return rotation;}
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
    Entity2D* GetChild(int index);
    void AddChild(Entity2D* newChild);
    void RemoveChild(Entity2D*);
    bool HasChild(Entity2D*);
    int GetChildCount();


private:
    float positionX = 0.0f;
    float positionY = 0.0f;

    float scaleX = 0.0f;
    float scaleY = 0.0f;

    /// @brief Rotation of this entity in radians
    float rotation = 0.0f;
    bool isReady = false;

    /// @brief The children of this entity
    std::unique_ptr<std::vector<Entity2D*> > children = std::make_unique<std::vector<Entity2D*>>();

};