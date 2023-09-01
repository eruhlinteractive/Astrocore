#ifndef __TRIGGER2D_H__
#define __TRIGGER2D_H__

#include "../../include/box2d/box2d.h"
#include "collisionEntity.h"

namespace Astrolib
{
    class Trigger2D : public CollisionEntity
    {
    public:
        /// @brief Create a 2D trigger with a rectangular trigger shape
        /// @param position The position of the trigger entity
        /// @param triggerOrigin The origin of the trigger fixture (collider)
        /// @param triggerSize The width/height of the trigger rectangle
        Trigger2D(Vector2 position, Vector2 triggerOrigin, Vector2 triggerSize);

        /// @brief Create a 2D trigger with a circular trigger shape
        /// @param position The position of the trigger entity
        /// @param triggerOrigin The local origin of the trigger shape
        /// @param radius The radius of the trigger shape
        Trigger2D(Vector2 position, Vector2 triggerOrigin, float radius);

        /// @brief Create a 2D trigger with a custom trigger shape
        /// @param position The starting position of the trigger body
        /// @param triggerShape The custom shape to apply to the trigger
        Trigger2D(Vector2 position, b2Shape *triggerShape);

        void AddCircleTrigger(Vector2 triggerOrigin, float radius);
        void AddRectangleTrigger(Vector2 triggerOrigin, Vector2 triggerSize);
        void AddCustomTrigger(b2Shape *triggerShape, Vector2 triggerCenter);
        void OnRegister(Scene *scene) override;
        
        /// @brief Is a body with name currently in this trigger
        /// @param name The name of the body to query
        /// @return Whether the body is in this trigger area
        bool IsBodyInArea(std::string name);
        
        // b2ContactListener overrides
        void BeginContact(b2Contact *contact) override;
        void EndContact(b2Contact *contact) override;

        ~Trigger2D();

    protected:
        // This is protected to prevent other classes from creating trigger body without a fixture
        Trigger2D();

        /// @brief Initializes the underlying physics body with a given body definition
        /// @param def
        void InitBody(Vector2 position);
        void DoThing(){
            Debug::Log("AH");
        }
        void AddFixtureToBody(b2FixtureDef *newFixture) override;

        std::map<std::string, b2Body *> bodiesInTrigger;


    };
}
#endif // __TRIGGER2D_H__