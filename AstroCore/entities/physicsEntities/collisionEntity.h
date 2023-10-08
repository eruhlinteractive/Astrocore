#ifndef __COLLISIONENTITY_H__
#define __COLLISIONENTITY_H__

#include <box2d/box2d.h>
#include "../baseEntities/entity.h"

namespace Astrocore
{
    /// @brief An abstract base class for all physical entities that interface with the
    ///         box2D library
    class CollisionEntity : public Entity2D
    {
    public:
        inline b2Body *GetPhysicsBody()
        {
            return physicsBody;
        }

        /// @brief Called when a contact begins with this body
        /// @param contact The contact information
        void CollisionStart(b2Contact *contact)
        {
            // This will be called on all derived classes
            BeginContact(contact);
        };

        /// @brief Called when a contact ends with this body
        /// @param contact The contact information
        void CollisionEnd(b2Contact *contact)
        {
            // This will be called on all derived classes
            EndContact(contact);
        };

        void SetCollisionLayers(uint16_t collisionLayerBits)
        {
            colFilter->categoryBits = collisionLayerBits;
            UpdateCollisionFilters();
        };
        void SetCollisionMask(uint16_t collisionMaskLayers)
        {
            colFilter->maskBits = collisionMaskLayers;
            UpdateCollisionFilters();
        };
        void SetCollisionGroupIndex(int16 groupIndex)
        {
            colFilter->groupIndex = groupIndex;
            UpdateCollisionFilters();
        };

    protected:
        CollisionEntity()
        {
            type = PHYSICAL;
            tempFixtures = std::vector<b2FixtureDef *>();
            colFilter = new b2Filter();
        }

        ~CollisionEntity()
        {
            tempFixtures.clear();
            delete colFilter;
            colFilter = nullptr;
        }

        /// @brief Initializes the underlying physics body with a given body definition
        /// @param def
        // virtual void InitBody(Vector2 position)=0;
        virtual void AddFixtureToBody(b2FixtureDef *newFixture) = 0;

        // Template function, will get called in all derived classes
        //  https://stackoverflow.com/questions/17324078/c-how-to-call-method-in-derived-class-from-base-class
        virtual void BeginContact(b2Contact *contact) = 0;
        virtual void EndContact(b2Contact *contact) = 0;

        b2BodyDef *bodyDef = nullptr;
        b2Body *physicsBody = nullptr;

        /// @brief A cache of fixtures that were defined before the physics body was created
        std::vector<b2FixtureDef *> tempFixtures;
        bool addedToPhysicsWorld = false;

        b2Filter *colFilter;
        /// Iterates through the fixtures updating the filter layers
        void UpdateCollisionFilters()
        {
            // We haven't registered with the physics world, don't update the collision layers yet
            //  the colliders will be updated when the body is updated
            if(!addedToPhysicsWorld)
            {
                return;
            }

            b2Fixture *fixtures = physicsBody->GetFixtureList();

            // Double check that there are fixtures on the body at this point (should be after init)
            Debug::Assert(fixtures != nullptr, "There are no fixtures on this body!");

            // Iterate through the fixtures, updating the collision bits
            while (fixtures != nullptr)
            {
                fixtures->SetFilterData(*colFilter);
                fixtures++;
            }
        }
    };

} // namespace Astrolib

#endif // __COLLISIONENTITY_H__