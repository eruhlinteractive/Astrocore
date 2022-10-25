#include "entity.h"
#include <math.h>
#include <iostream>

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
}

/// @brief Rotate the entity by rotRad radians
/// @param rotRad The delta rotation in radians
void Entity2D::Rotate(float rotRad)
{
    std::cout<< GetRotation() << std::endl;
    rotation += rotRad;

    // Keep within range 0-> 2PI
    rotation = std::fmod(rotation, (2.0f * M_PI));
}

/// @brief Rotate the entity by rotDeg degrees
/// @param rotRad The delta rotation in radians
void Entity2D::RotateDeg(float rotdeg)
{
    std::cout<< GetRotation() << std::endl;    
    rotation += (rotdeg * ((float)M_PI / 180.0f));

    

    // Keep within range 0-> 2PI
    rotation = std::fmod(rotation, (2.0f * M_PI));
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
#pragma region Children Functions


/// @brief Update the entity
/// @param deltaTime The time since the previous frame
void Entity2D::Update(float deltaTime)
{
    if(!isReady)
    {
        Ready();
    }
}
#pragma endregion