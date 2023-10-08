#include "physicsEntity.h"

using namespace Astrocore;

PhysicsEntity::PhysicsEntity(PHYSICS_TYPE bodyType, Vector2 startPos)
{
    bodyDef = new b2BodyDef();
    bodyDef->position = b2Vec2(startPos.x, startPos.y);

    type = PHYSICAL;
    if (bodyType == STATIC)
    {
        bodyDef->type = b2_staticBody;
    }
    else if (bodyType == DYNAMIC)
    {
        bodyDef->type = b2_dynamicBody;
    }
    else if (bodyType == KINEMATIC)
    {
        bodyDef->type = b2_kinematicBody;
    }

    // TODO: Move this to when the entity gets added to the tree
    // Register physics body with world
    // physicsBody;
    transform.position = startPos;
}

void PhysicsEntity::OnRegister(Scene *scene)
{
    // Create and assign custom data to the physics body
    b2BodyUserData data = b2BodyUserData();
    data.pointer = (uintptr_t)(new PhysicsEntityData(entityID, name));
    bodyDef->userData = data;

    // Register physics body to the scene
    physicsBody = scene->GetPhysicsWorld()->CreateBody(bodyDef);

    addedToPhysicsWorld = true;
    currentScene = scene;

    // Add all cached fixtures to the body
    for (b2FixtureDef *fixDef : fixtureTemps)
    {
        AddFixtureToBody(fixDef);

        // Cleanup memory since the pointer was duplicated
        // Note: Since all fixtures are created internally via helper functions,
        //  it is assumed they can be safely deleted
        delete fixDef->shape;
        fixDef->shape = nullptr;

        delete fixDef;
        fixDef = nullptr;
    }

    // Clear out the array, since it will no longer be used
    fixtureTemps.clear();
}

PhysicsEntity::PhysicsEntity(std::string name, PHYSICS_TYPE bodyType, Vector2 position, float rotation)
    : PhysicsEntity(bodyType, position)
{
    this->name = name;
    physicsBody->SetTransform(b2Vec2(position.x, position.y), rotation);
}

PhysicsEntity::~PhysicsEntity()
{
    if (addedToPhysicsWorld)
    {
        currentScene->GetPhysicsWorld()->DestroyBody(physicsBody);
    }

    fixtureTemps.clear();
}

#pragma region ColliderFunctions

/// @brief Creates a rectangle collider with no rotation and a given size and center point, using
/// default values for density, friction, and restitution
/// @param center The center of the rectangle (relative to the body)
/// @param size The size of the rectangle (width and height)
void PhysicsEntity::CreateRectangleCollider(Vector2 center, Vector2 size)
{
    CreateRectangleCollider(center, size, 0.0f, 0.1f, 0.1f, 0.5f);
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
    if (physicsBody->GetType() == b2_staticBody || physicsBody->GetType() == b2_kinematicBody)
    {
        // TODO: Figure out how to convert from meters to pixels per Box2d documentation
        // https://box2d.org/documentation/md__d_1__git_hub_box2d_docs_loose_ends.html#autotoc_md124

        Vector2 globalPos = GetGlobalPosition();
        physicsBody->SetTransform(b2Vec2{globalPos.x, globalPos.y}, -transform.rotation);
    }
    else if (physicsBody->GetType() == b2_dynamicBody)
    {
        // Update entity properties/transforms to match physics transforms
        b2Vec2 pos = physicsBody->GetTransform().p;
        transform.position = (Vector2){pos.x, pos.y};
        transform.rotation = physicsBody->GetAngle();
        transform.scale = (Vector2){1.0, 1.0};
    }
}

/// @brief Creates a rectangle body and associated fixture
/// @param center The center of the collider relative to the body
/// @param size The length/width of the body
/// @param rotRads The local rotation of the collider in radians
/// @param density The density of the fixture
/// @param friction The friction of the fixture
/// @param restitution The restitution (bounciness) of the fixture
void PhysicsEntity::CreateRectangleCollider(Vector2 center,
                                            Vector2 size,
                                            float rotRads,
                                            float density = 1,
                                            float friction = 0.2f,
                                            float restitution = 0.0f)
{
    // Create the box shape
    b2PolygonShape *shape = new b2PolygonShape();
    shape->SetAsBox(size.x / 2.0, size.y / 2.0, b2Vec2(center.x, center.y), rotRads);

    // Define the fixture and add it to the body
    b2FixtureDef *fix = new b2FixtureDef();
    fix->shape = shape;
    fix->density = density;
    fix->friction = friction;
    fix->restitution = restitution;
    AddFixtureToBody(fix);
}

Vector2 PhysicsEntity::GetGlobalPosition()
{
    if (physicsBody->GetType() == b2_kinematicBody || physicsBody->GetType() == b2_staticBody)
    {
        return Entity2D::GetGlobalPosition();
    }
    else
    {
        b2Vec2 pos = physicsBody->GetPosition();
        return (Vector2){pos.x, pos.y};
    }
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
    b2CircleShape *circle;
    circle->m_p.Set(center.x, center.y);
    circle->m_radius = radius;

    // Define the fixture and add it to the body
    b2FixtureDef *fix = new b2FixtureDef();
    fix->shape = circle;
    fix->density = density;
    fix->friction = friction;
    fix->restitution = restitution;
    AddFixtureToBody(fix);
}

void PhysicsEntity::CreateCircleCollider(Vector2 center, float radius)
{
    CreateCircleCollider(center, radius, 1.0f, 0.2f, 0.0f);
}

void PhysicsEntity::CreateConvexPolyCollider(std::vector<Vector2> points)
{
    CreateConvexPolyCollider(points, 0.0f, 1.0f, 0.2f, 0.0f);
}

void PhysicsEntity::CreateConvexPolyCollider(std::vector<Vector2> points, float rotRads = 0.0f,
                                             float density = 1.0f,
                                             float friction = 0.2f,
                                             float restitution = 0.0f)
{
    std::vector<b2Vec2> b2Points = std::vector<b2Vec2>();
    for (Vector2 point : points)
    {
        // Convert to b2d coordinates
        point.y *= -1;
        b2Points.push_back(b2Vec2(point.x, point.y));
    }

    b2PolygonShape* newShape = new b2PolygonShape();
    newShape->Set(b2Points.data(), b2Points.size());

    // Define the fixture and add it to the body
    b2FixtureDef *fix = new b2FixtureDef();
    fix->shape = newShape;
    fix->density = density;
    fix->friction = friction;
    fix->restitution = restitution;
    AddFixtureToBody(fix);
}

void PhysicsEntity::AddFixtureToBody(b2FixtureDef *fixtureDefinition)
{
    // Add the fixture directly if it has already been added to the physics world,
    //  otherwise store it to be added when the physics body is created
    if (physicsBody != nullptr && physicsBody != 0)
    {
        b2Fixture* fix = physicsBody->CreateFixture(fixtureDefinition);
        fix->SetFilterData(*colFilter);
    }
    else
    {
        fixtureTemps.push_back(fixtureDefinition);
    }
}

#pragma endregion

#pragma region Manipulation Functions

/// @brief Apply a force to the center of this body
/// @param force The force vector to apply
void PhysicsEntity::AddForce(Vector2 force)
{
    physicsBody->ApplyForceToCenter(b2Vec2(-force.x, -force.y), true);
}

void PhysicsEntity::AddForceAtPoint(Vector2 force, Vector2 point)
{
    physicsBody->ApplyForce(b2Vec2(-force.x, -force.y), b2Vec2(point.x, point.y), true);
}

void PhysicsEntity::ApplyTorque(float torque)
{
    physicsBody->ApplyTorque(-torque, true);
}

void PhysicsEntity::AddLocalForce(Vector2 force)
{
    float globalRot = physicsBody->GetAngle();

    Vector2 localForce = {
        cos(globalRot) * force.x - sin(globalRot) * force.y,
        cos(globalRot) * force.y + sin(globalRot) * force.x,
    };
    physicsBody->ApplyForceToCenter(b2Vec2(-localForce.x, -localForce.y), true);
}

void PhysicsEntity::ApplyImpulse(Vector2 force)
{
    physicsBody->ApplyLinearImpulseToCenter(b2Vec2(-force.x, -force.y), true);
}

void PhysicsEntity::ApplyImpulseAtPoint(Vector2 force, Vector2 point)
{
    physicsBody->ApplyLinearImpulse(b2Vec2(-force.x, -force.y), b2Vec2(point.x, point.y), true);
}

#pragma endregion