#ifndef __RAYCAST_H__
#define __RAYCAST_H__

#include <box2d/box2d.h>
#include "../data/structs.h"

namespace Astrocore
{

struct RaycastInfo
{
    /// @brief The point of the collision
    Vector2 colPoint;
    
    /// @brief The normal of the edge at the point of the collision
    Vector2 colNormal;

    int entityID;
    std::string entityName;

    RaycastInfo(b2Vec2 point, b2Vec2 normal, PhysicsEntityData data)
    {
        colPoint = (Vector2){point.x, point.y};
        colNormal = (Vector2){normal.x, normal.y};
        entityID = data.entityID;
        entityName = data.entityName;
    }
};

class Raycast : public b2RayCastCallback
{
    
public:
    Raycast()
    {
        results = std::vector<RaycastInfo>();
    }
 
    float ReportFixture(b2Fixture* fixture, const b2Vec2& point,
                        const b2Vec2& normal, float fraction)
    {
        // If the raycast isn't checking this layer, return -1 to continue the ray 
        //  Bitwise AND
        if((collisionFilter & fixture->GetFilterData().categoryBits) == 0)
        {
            return -1;
        }

        b2Body* body = fixture->GetBody();
        PhysicsEntityData *bodyData = (PhysicsEntityData *)(body->GetUserData().pointer);

        // Add to results array
        results.push_back(RaycastInfo(point, normal, *bodyData));
        
        // If we've exceeded our max size, return 0 to terminate the ray cast
        if(maxCollisions >= results.size())
        {
            return 0;
        }

        return fraction;
    }

    std::vector<RaycastInfo> FireRayCast(b2World* physicsWorld, Vector2 startPoint, Vector2 endPoint, uint16_t collisionMask = 0x0000)
    {
        b2Vec2 start = b2Vec2(startPoint.x, startPoint.y);
        b2Vec2 end = b2Vec2(endPoint.x, endPoint.y);

        collisionFilter = collisionMask;
        
        results.clear();

        // Run raycast query
        physicsWorld->RayCast(this, start, end);
        
        return results;
    };
 
    uint16_t collisionFilter = 0x0000;
    int maxCollisions = 1;
    std::vector<RaycastInfo> results;
};
}
#endif // __RAYCAST_H__