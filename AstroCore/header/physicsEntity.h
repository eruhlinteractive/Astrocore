#ifndef __PHYSICSENTITY_H__
#define __PHYSICSENTITY_H__

#include "entity.h"
#include "../include/box2d/box2d.h"
#include "game.h"

namespace Astrolib
{
    enum PHYSICS_TYPE
    {
        STATIC, // Doesn't move
        DYNAMIC, // Moveable
        KINEMATIC // Moveable by user
    };

    class PhysicsEntity : public Entity2D
    {
    public:
        PhysicsEntity(PHYSICS_TYPE bodyType, Vector2 startPos);
        PhysicsEntity(std::string name, PHYSICS_TYPE bodyType, Vector2 position, float rotation);
        ~PhysicsEntity();

        virtual void FixedUpdate(float deltaTime) override;
        void OnRegister(Scene* scene) override;

        // Helper functions for creating collision polygons

        void CreateRectangleCollider(Vector2 center, Vector2 size);
        void CreateRectangleCollider(Vector2 center, Vector2 size, float rotRads);
        void CreateRectangleCollider(
            Vector2 center,
            Vector2 size,
            float rotRads,
            float density,
            float friction,
            float restitution);

        void CreateCircleCollider(
            Vector2 center,
            float radius,
            float density,
            float friction,
            float restitution);
            
        void CreateCircleCollider(Vector2 center, float radius);

        /// @brief Add a new fixture (collider) to this body
        /// @param fixtureDefinition A pointer to the fixture definition (can also be the address of a stack variable)
        void AddFixtureToBody(b2FixtureDef* fixtureDefinition);
        
        inline b2Body* GetPhysicsBody()
        {
            return physicsBody;
        }

        Vector2 GetGlobalPosition() override;
        // Manipulation functions
        void AddForce(Vector2 force);
        void AddForceAtPoint(Vector2 force, Vector2 point);
        void ApplyTorque(float torque);

        void ApplyImpulse(Vector2 force);
        void ApplyImpulseAtPoint(Vector2 force, Vector2 point);

    protected:
        b2BodyDef bodyDef;
        b2Body *physicsBody;
        PHYSICS_TYPE physicsMode = STATIC;
        bool addedToPhysicsWorld = false;

        /// @brief Holds fixtures defined before the body has been added to the physics world
        std::vector<b2FixtureDef*> fixtureTemps;
    };

};
#endif // __PHYSICSENTITY_H__