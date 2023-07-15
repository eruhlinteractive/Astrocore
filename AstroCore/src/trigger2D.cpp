#include "../header/trigger2D.h"
#include "../header/scene.h"

using namespace Astrolib;

Trigger2D::Trigger2D(Vector2 position, Vector2 triggerOrigin, Vector2 triggerSize)
{
    InitBody(position);

    // Create box shape
    b2PolygonShape shape;
    shape.SetAsBox(triggerSize.x / 2.0f, triggerSize.y / 2.0f, (b2Vec2){triggerOrigin.x, triggerOrigin.y}, 0.0f);

    // Create sensor (trigger) fixture
    b2FixtureDef fix;
    fix.shape = &shape;
    fix.isSensor = true;
    physicsBody->CreateFixture(&fix);
}

Trigger2D::Trigger2D(Vector2 position, Vector2 triggerOrigin, float radius)
{
    InitBody(position);

    // Create box shape
    b2CircleShape shape;
    shape.m_p = b2Vec2(triggerOrigin.x, triggerOrigin.y);
    shape.m_radius = radius;

    // Create sensor (trigger) fixture
    b2FixtureDef fix;
    fix.shape = &shape;
    fix.isSensor = true;
    physicsBody->CreateFixture(&fix);
}


Trigger2D::Trigger2D(Vector2 position, b2Shape *triggerShape)
{
    InitBody(position);

}

void Trigger2D::InitBody(Vector2 position)
{
    // Create new body definition
    bodyDef = b2BodyDef();
    bodyDef.type = b2_kinematicBody;
    bodyDef.position = b2Vec2(position.x, position.y);
}

void Trigger2D::OnRegister(Scene* scene)
{
    // Don't simulate the trigger until added to the scene
    physicsBody = scene->GetPhysicsWorld()->CreateBody(&bodyDef);
}
