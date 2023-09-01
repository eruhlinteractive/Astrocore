#ifndef __COLLISIONENTITY_H__
#define __COLLISIONENTITY_H__

#include "../../include/box2d/box2d.h"
#include "../baseEntities/entity.h"

namespace Astrolib
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
        void CollisionEnd(b2Contact *contact){
            // This will be called on all derived classes
            EndContact(contact);
        };

    protected:
        CollisionEntity()
        {
            type = PHYSICAL;
            tempFixtures = std::vector<b2FixtureDef *>();
        }

        ~CollisionEntity()
        {
            tempFixtures.clear();
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
    };

} // namespace Astrolib

#endif // __COLLISIONENTITY_H__