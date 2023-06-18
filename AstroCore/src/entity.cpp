#include "../header/entity.h"

#include <iostream>
#include <stdexcept>
#include <algorithm>

using namespace Astrolib;

int Entity2D::entityCount = 0;

#pragma region Constructors
Entity2D::Entity2D()
{
    type = BASE;
    transform = Transform2D();

    // Set id and increment entity count
    entityID = entityCount++;
    
    name = "Entity2D" + std::to_string(entityID);
    Init();
}

Entity2D::Entity2D(std::string name) : Entity2D()
{
    this->name = name; 
}

Entity2D::Entity2D(std::string name, Vector2 startPosition, Vector2 startScale, float startRotation): Entity2D(name)
{
    transform = Transform2D(startPosition, startRotation, startScale);
    Init();
}


Entity2D::~Entity2D()
{
    OnDestroy();
}
#pragma endregion

#pragma region Transformation Functions

/// @brief Get the position of this entity in world space
/// @return A Vector2 representing the world coordinates
Vector2 Entity2D::GetGlobalPosition()
{
    if(this->parentEntity != nullptr)
    {
        
        Vector2 parentGlobal = parentEntity->GetGlobalPosition();
        // Apply parent's rotation to position 
        // https://academo.org/demos/rotation-about-point/
        float parentRotation = parentEntity->GetGlobalRotation();

        Vector2 rotatedPos = (Vector2)
        {
            transform.position.x * cos(parentRotation) - transform.position.y * sin(parentRotation),
            transform.position.y * cos(parentRotation) + transform.position.x * sin(parentRotation),
        };

        return (Vector2){parentGlobal.x + rotatedPos.x, parentGlobal.y + rotatedPos.y};
        //return GetPosition();
    }
    else
    {
        Vector2 globalPos = transform.position;
        return globalPos;
    }
}

/// @brief Get the rotation of this entity in global space (Degrees)
/// @return The rotation in degrees
float Entity2D::GetGlobalRotationDeg()
{
    return (GetGlobalRotation() * (180.0/M_PI));
}

/// @brief Get the rotation of this entity in global space in radians
/// @return The global space rotation of this entity
float Entity2D::GetGlobalRotation()
{
    float globalRot = transform.rotation;

    if(parentEntity != nullptr)
    {
        globalRot = transform.rotation + parentEntity->GetGlobalRotation();
        globalRot = std::fmod(globalRot, (2.0f * M_PI));
    }

    return globalRot;
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

std::string Entity2D::GetPath()
{
    std::string path = GetName();

    Entity2D* current = GetParent();
    while(current != nullptr)
    {
        path = current->GetName() + "/" + path;
        current = current->GetParent();
    }
    return path;
}


/// @brief Get the amount of children this entity has
/// @return Int of how many child entities this entity has
int Entity2D::GetChildCount()
{
    return children->size();
}

/// @brief Get a child of this node by name
/// @param name The name of the child to get
/// @return The pointer to the entity
Entity2D* Entity2D::GetChild(std::string name)
{
    for(Entity2D* child : *children)
    {
        if(child->GetName() == name)
        {
            return child;
        }
    }

    return nullptr;
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
    //std::cout << children->size() << std::endl;
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

std::vector<Entity2D*> Entity2D::GetChildren()
{
    return *children;
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