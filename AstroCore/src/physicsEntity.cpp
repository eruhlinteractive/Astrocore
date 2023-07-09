#include "../header/physicsEntity.h"

using namespace Astrolib;

PhysicsEntity::PhysicsEntity(PHYSICS_TYPE bodyType)
{
    bodyDef = b2BodyDef();
    bodyDef.position.SetZero();

    type = PHYSICAL;
    if (bodyType == STATIC)
    {
        bodyDef.type = b2_staticBody;
    }
    else if (bodyType == DYNAMIC)
    {
        bodyDef.type = b2_dynamicBody;
    }

    // Register physics body with world
    physicsBody = Game::instance().GetPhysicsWorld()->CreateBody(&bodyDef);
}

PhysicsEntity::PhysicsEntity(std::string name, PHYSICS_TYPE bodyType, Vector2 position, float rotation)
    : PhysicsEntity(bodyType)
{
    this->name = name;
    physicsBody->SetTransform(b2Vec2(position.x, position.y), rotation);
}

PhysicsEntity::~PhysicsEntity()
{
}

/// @brief Creates a rectangle collider with no rotation and a given size and center point, using
/// default values for density, friction, and restitution
/// @param center The center of the rectangle (relative to the body)
/// @param size The size of the rectangle (width and height)
void PhysicsEntity::CreateRectangleCollider(Vector2 center, Vector2 size)
{
    CreateRectangleCollider(center, size, 0.0f, 1.0f, 0.1f, 0.5f);
}

/// @brief Creates a rectangle collider with a relative rotation and a given size and center point, using
/// default values for density, friction, and restitution
/// @param center The center of the rectangle (relative to the body)
/// @param size The size of the rectangle (width and height)
/// @param rotRads The rotation of the rectangle in radians
void PhysicsEntity::CreateRectangleCollider(Vector2 center, Vector2 size, float rotRads)
{
    CreateRectangleCollider(center, size, rotRads, 1.0f, 0.1f, 0.5f);
}

void PhysicsEntity::FixedUpdate(float deltaTime)
{
    // Update entity properties/transforms to match physics transforms
    b2Vec2 pos = physicsBody->GetLocalPoint(physicsBody->GetPosition());
    transform.position = (Vector2){pos.x, pos.y};
    transform.rotation = physicsBody->GetAngle();
    transform.scale = (Vector2){1.0, 1.0};
}

/// @brief Creates a rectangle body and
/// @param center
/// @param size
/// @param rotRads
/// @param density
/// @param friction
/// @param restitution
void PhysicsEntity::CreateRectangleCollider(Vector2 center,
                                            Vector2 size,
                                            float rotRads,
                                            float density = 1,
                                            float friction = 0.2f,
                                            float restitution = 0.0f)
{
    // Create the box shape
    b2PolygonShape shape;
    shape.SetAsBox(size.x / 2.0, size.y / 2.0, b2Vec2(center.x, center.y), rotRads);

    // Define the fixture and add it to the body
    b2FixtureDef fix;
    fix.shape = &shape;
    fix.density = density;
    fix.friction = friction;
    fix.restitution = restitution;
    AddFixtureToBody(fix);
}

/// @brief Creates a circle collider and adds it as a fixture to the physics body
/// @param center The center of the circle, relative to the body
/// @param radius The radius of the circle
/// @param density The density of the fixture
/// @param friction The friction of the fixture
/// @param restitution the restitution of the fixture
void PhysicsEntity::CreateCircleCollider(
    Vector2 center,
    float radius,
    float density = 1.0f,
    float friction = 0.2f,
    float restitution = 0.0f)
{
    b2CircleShape circle;
    circle.m_p.Set(center.x, center.y);
    circle.m_radius = radius;

    // Define the fixture and add it to the body
    b2FixtureDef fix;
    fix.shape = &circle;
    fix.density = density;
    fix.friction = friction;
    fix.restitution = restitution;
    AddFixtureToBody(fix);
}

void PhysicsEntity::CreateCircleCollider(Vector2 center, float radius)
{
    CreateCircleCollider(center, radius, 1.0f, 0.2f, 0.0f);
}

void PhysicsEntity::AddFixtureToBody(b2FixtureDef fixtureDefinition)
{
    physicsBody->CreateFixture(&fixtureDefinition);
}

#pragma region Manipulation Functions

/// @brief Apply a force to the center of this body
/// @param force The force vector to apply
inline void PhysicsEntity::AddForce(Vector2 force)
{
    physicsBody->ApplyForceToCenter(b2Vec2(force.x, force.y), true);
}

inline void PhysicsEntity::AddForceAtPoint(Vector2 force, Vector2 point)
{
    physicsBody->ApplyForce(b2Vec2(force.x, force.y), b2Vec2(point.x, point.y), true);
}

inline void PhysicsEntity::ApplyTorque(float torque)
{
    physicsBody->ApplyTorque(torque, true);
}


inline void PhysicsEntity::ApplyImpulse(Vector2 force)
{
    physicsBody->ApplyLinearImpulseToCenter(b2Vec2(force.x, force.y), true);
}


inline void PhysicsEntity::ApplyImpulseAtPoint(Vector2 force, Vector2 point)
{
    physicsBody->ApplyLinearImpulse(b2Vec2(force.x, force.y), b2Vec2(point.x, point.y), true);
}

#pragma endregion