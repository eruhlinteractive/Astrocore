#ifndef __COLLISIONTRACKER_H__
#define __COLLISIONTRACKER_H__

#include "../include/box2d/box2d.h"
#include "../header/collisionEntity.h"

namespace Astrolib
{
    /// This class listens for collisions in the physics world and notifies the correct entities
    ///     Note: This is not meant to be queried directly, except by the scene to add/remove entities
    class CollisionTracker : public b2ContactListener
    {
    public:
        CollisionTracker()
        {
            collisionEntityMap = std::map<int, CollisionEntity*>();
        }

        void AddTrackedEntity(int entityID, CollisionEntity* entity);
        void RemoveTrackedEntity(int entityID, CollisionEntity* entity);
        bool IsTrackingEntity(int entityID);

        void BeginContact (b2Contact *contact) override;
        void EndContact (b2Contact *contact) override;

    private:
        std::map<int, CollisionEntity*> collisionEntityMap;
    };
}
#endif // __COLLISIONTRACKER_H__