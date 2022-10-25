#include "entity.h"
#include <math.h>
#include <iostream>
#include <stdexcept>
#include <algorithm>

#pragma region Constructors
Entity2D::Entity2D()
{
    positionX = 0.0f;
    positionY = 0.0f;
    rotation = 0.0f;

    scaleX = 1.0f;
    scaleY = 1.0f;
    Init();
}

Entity2D::Entity2D(Vector2 startPosition, Vector2 startScale, float startRotation)
{
    positionX = startPosition.x;
    positionY = startPosition.y;
    rotation = startRotation;

    scaleX = startScale.x;
    scaleY = startScale.y;
    Init();
}

Entity2D::~Entity2D()
{
    OnDestroy();
}
#pragma endregion

#pragma region Transformation Functions

/// @brief Move the position in the global space
/// @param movement The vector of movement
void Entity2D::MoveGlobal(Vector2 movement)
{
    positionX += movement.x;
    positionY += movement.y;

    // Update child transforms
    for(Entity2D* child : *children)
    {
        child->MoveGlobal(movement);
    }
}

/// @brief Move the position realtive to the rotation
/// @param movement The vector of movement
void Entity2D::MoveLocal(Vector2 movement)
{

    Vector2 movLocal = movement;
    movLocal.x = movement.x * cos(rotation) - movement.y * sin(rotation);
    movLocal.y = movement.x * sin(rotation) + movement.y * cos(rotation);

    //std::cout<< movLocal.x << ":" << movLocal.y << std::endl;
    
    positionX += movLocal.x;
    positionY += movLocal.y;

    // Update child transforms
    for(Entity2D* child : *children)
    {
        child->MoveGlobal(movLocal);
    }

}

/// @brief Get the position of this entity in world space
/// @return A Vector2 representing the world coordinates
Vector2 Entity2D::GetGlobalPosition()
{
    if(parentEntity != nullptr)
    {
        Vector2 parentGlobal = parentEntity->GetGlobalPosition();
        return {positionX + parentGlobal.x, positionY + parentGlobal.y};
    }
    else
    {
        return GetPosition();
    }
}

/// @brief Rotate this object around a point
/// @param rotDeg The angle (degrees) to rotate by
/// @param point The point to rotate around
void Entity2D::RotateDegAroundPoint(float rotDeg, Vector2 point)
{
    float rotRad = (rotDeg * ((float)M_PI / 180.0f));
    RotateAroundPoint(rotRad, point);
   
    //MoveGlobal(translation);   
}

void Entity2D::RotateAroundPoint(float rotRad, Vector2 point)
{
    Vector2 translation = {positionX - point.x, positionY - point.y};

    float xNew = translation.x * cos(rotRad) - translation.y * sin(rotRad);
    float yNew = translation.x * sin(rotRad) + translation.y * cos(rotRad);

    translation = {xNew + point.x, yNew + point.y};


    Vector2 movement = {translation.x - positionX, translation.y - positionY};
    
    

    MoveGlobal(movement);
    for(Entity2D* child : *children)
    {
        child->RotateAroundPoint(rotRad, {positionX, positionY});
    }
    //SetPosition(movement);
    //Rotate(rotRad);
    rotation += rotRad;
}


/// @brief Rotate the entity by rotRad radians
/// @param rotRad The delta rotation in radians
void Entity2D::Rotate(float rotRad)
{
    //std::cout<< GetRotation() << std::endl;
    rotation += rotRad;

    for (Entity2D* child : *children)
    {
        child->RotateAroundPoint(rotRad, GetPosition());
        //child->Rotate(rotRad);
    }

    // Keep within range 0-> 2PI
    rotation = std::fmod(rotation, (2.0f * M_PI));
}

/// @brief Rotate the entity by rotDeg degrees
/// @param rotRad The delta rotation in radians
void Entity2D::RotateDeg(float rotDeg)
{
    //std::cout<< GetRotation() << std::endl;    
    Rotate((rotDeg * ((float)M_PI / 180.0f)));
}

/// @brief Set the position of this entity
/// @param newPosition The new position to set
void Entity2D::SetPosition(Vector2 newPosition)
{
    positionX = newPosition.x;
    positionY = newPosition.y;
}

/// @brief Set the scale of this entity
/// @param newScale The new scale to set
void Entity2D::SetScale(Vector2 newScale)
{
    scaleX = newScale.x;
    scaleY = newScale.y;
}

/// @brief Set the rotation of this entity in radians
/// @param newRotation The new rotation to set (radians)
void Entity2D::SetRotation(float newRotation)
{
    for(Entity2D* child: *children)
    {
        child->RotateAroundPoint(rotation - newRotation, GetPosition());
    }
    rotation = newRotation;

    // Keep within range 0-> 2PI
    rotation = std::fmod(rotation, (2.0f * M_PI));
}

/// @brief Set the rotation of this entity in degrees
/// @param newRotation The new rotation to set (degrees)
void Entity2D::SetRotationDeg(float newRotationDeg)
{
    rotation = (newRotationDeg * M_PI / 180);

    // Keep within range 0-> 2PI
    rotation = std::fmod(rotation, (2.0f * M_PI));
}

#pragma endregion

/// @brief Update the entity
/// @param deltaTime The time since the previous frame
void Entity2D::Update(float deltaTime)
{
    if(!isReady)
    {
        Ready();
    }
}

#pragma region Children Functions

/// @brief Get the parent of this object
/// @return The raw ptr to the object, or nullptr if it doesn't exist
Entity2D* Entity2D::GetParent()
{
    return parentEntity;
}

/// @brief Get the amount of children this entity has
/// @return Int of how many child entities this entity has
int Entity2D::GetChildCount()
{
    return children->size();
}

/// @brief Get the child ptr of a child at a specific index
/// @param index The index of the child in the vector
/// @return A pointer to the child entity
Entity2D* Entity2D::GetChildAtIndex(int index)
{
    if(children->size() < index)
    {
        return (*children)[index];
    }

    // Throw error, index out of bounds
    throw std::invalid_argument("Child index out of bounds");
}

/// @brief Set the parent of this entity
/// @param newParent The note to set as the new parent
void Entity2D::SetParent(Entity2D* newParent)
{
    // Remove as child from previous parent before updating
    if(parentEntity != nullptr)
    {
        parentEntity->RemoveChild(this);
    }

    parentEntity = newParent;
    parentEntity->AddChild(this);
}

/// @brief Add and entity as a child of this entity
/// @param newChild The new child to add to this entity
void Entity2D::AddChild(Entity2D* newChild)
{
    std::cout << children->size() << std::endl;
    if(newChild->GetParent() != this)
    {
        // TODO: Swap parents, or throw error
        Entity2D* currentParent = newChild->GetParent();

        if(currentParent != nullptr)
        {
            currentParent->RemoveChild(newChild);
        }
    }
    if(!HasChild(newChild))
    {
        children->push_back(newChild);
        newChild->SetParent(this);
    }
}

/// @brief Checks if the entity is a child of this entity
/// @param childToCheck The potential child in question
/// @return True if the entity is a child, otherwise false
bool Entity2D::HasChild(Entity2D* childToCheck)
{
    auto index = find(children->begin(), children->end(), childToCheck);
    return index != children->end();
}

/// @brief Removes the entity as a child of this entity
/// @param childToRemove The child that should be removed from this entity
void Entity2D::RemoveChild(Entity2D* childToRemove)
{
    auto index = find(children->begin(), children->end(), childToRemove);
    if(index != children->end())
    {
        children->erase(index);
        childToRemove->SetParent(nullptr);
    }
}
#pragma endregion