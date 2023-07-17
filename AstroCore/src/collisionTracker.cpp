#include "../header/collisionTracker.h"

using namespace Astrolib;

void CollisionTracker::AddTrackedEntity(int entityID, CollisionEntity* entity)
{
    if(collisionEntityMap.find(entityID) == collisionEntityMap.end())
    {
        collisionEntityMap.insert({entityID, entity});
    }
}

void CollisionTracker::RemoveTrackedEntity(int entityID, CollisionEntity* entity)
{
    if(collisionEntityMap.find(entityID) != collisionEntityMap.end())
    {
        collisionEntityMap.erase(entityID);
    }
}

bool CollisionTracker::IsTrackingEntity(int entityID)
{
    return collisionEntityMap.find(entityID) != collisionEntityMap.end();
}

void CollisionTracker::BeginContact(b2Contact *contact)
{
    //return;
    b2Body* bodyA = contact->GetFixtureA()->GetBody();
    b2Body* bodyB = contact->GetFixtureB()->GetBody();

    // Bail if we get bad data
    if(bodyA == nullptr || bodyB == nullptr)
    {
        return;
    }

    PhysicsEntityData *dataA = (PhysicsEntityData *)(bodyA->GetUserData().pointer);
    PhysicsEntityData *dataB = (PhysicsEntityData *)(bodyB->GetUserData().pointer);

    // Notify collision participants
    if(dataA != nullptr)
    {
        collisionEntityMap[dataA->entityID]->CollisionStart(contact);
    }

    if(dataB != nullptr)
    {
        collisionEntityMap[dataB->entityID]->CollisionStart(contact);
    }

}

void CollisionTracker::EndContact(b2Contact *contact)
{
    //return;
    b2Body* bodyA = contact->GetFixtureA()->GetBody();
    b2Body* bodyB = contact->GetFixtureB()->GetBody();

    // Bail if we get bad data
    if(bodyA == nullptr || bodyB == nullptr)
    {
        return;
    }

    PhysicsEntityData *dataA = (PhysicsEntityData *)(bodyA->GetUserData().pointer);
    PhysicsEntityData *dataB = (PhysicsEntityData *)(bodyB->GetUserData().pointer);

    // Notify collision participants
    if(dataA != nullptr)
    {
        collisionEntityMap[dataA->entityID]->CollisionEnd(contact);
    }

    if(dataB != nullptr)
    {
        collisionEntityMap[dataB->entityID]->CollisionEnd(contact);
    }
}
