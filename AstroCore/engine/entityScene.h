#ifndef __ENTITYSCENE_H__
#define __ENTITYSCENE_H__

// This is a header only 'interface' that provides access to entity available scene data and functions,
//  but doesn't provide implementation details
//  NOTE: This should not be inherited when creating a new scene. Instead use scene.h
#include <raylib.h>



namespace Astrolib
{
    class Entity2D;
    class CameraEntityBase;
    class EntityScene
    {
    public:
        /// @brief Check if the entity at screenPosition is visible on screen
        /// @param camera The camera that is currently rendering the screen
        /// @param entityRect The rectangle of the entity
        /// @return True if the AABB check passes and the object is visible on screen
        virtual bool IsOnScreen(Rectangle entityRect) = 0;
       
        
        virtual Entity2D *FindEntityByName(std::string name) = 0;
        virtual Entity2D *GetEntity(std::string path)= 0;

        virtual Vector2 GetWorldRenderSize() = 0;

        virtual b2World *GetPhysicsWorld() = 0;

        /// @brief Register a resource with a given name (service locator pattern)
        /// @param name The name of the resource to register
        /// @param resource A pointer to the resource to register
        virtual void RegisterResource(string name, void* resource) = 0;

        /// @brief Get a resource with a given name
        /// @param name The name of the resource to get
        /// @return A void pointer to the resource, or nullptr if the resource hasn't been registered yet
        virtual void* GetResource(string name) = 0;
        virtual bool HasResource(string name)= 0;

        virtual Entity2D *GetRoot() = 0;
        virtual CameraEntityBase* GetCurrentCamera() = 0;
        virtual void SetCurrentCamera(CameraEntityBase* newCamera) = 0;
        virtual float GetPhysicsFraction() = 0;

    };
}

#endif // __ENTITYSCENE_H__