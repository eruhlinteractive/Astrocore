#include "trigger2D.h"
#include "../../engine/scene.h"

using namespace Astrolib;

Trigger2D::Trigger2D()
{
    tempFixtures = std::vector<b2FixtureDef *>();
}

Trigger2D::~Trigger2D()
{
    // If this was added to the world, destroy the body
    if (addedToPhysicsWorld)
    {
        currentScene->GetPhysicsWorld()->DestroyBody(physicsBody);
    }

    // Memory cleanup
    delete bodyDef;
    bodyDef = nullptr;

    tempFixtures.clear();
}

Trigger2D::Trigger2D(Vector2 position, Vector2 triggerOrigin, Vector2 triggerSize)
{
    InitBody(position);

    // Create box shape
    b2PolygonShape *shape = new b2PolygonShape();
    shape->SetAsBox(triggerSize.x / 2.0f, triggerSize.y / 2.0f, (b2Vec2){triggerOrigin.x, triggerOrigin.y}, 0.0f);

    // Create sensor (trigger) fixture
    b2FixtureDef *fix = new b2FixtureDef();
    fix->shape = shape;
    fix->isSensor = true;
    AddFixtureToBody(fix);
}

Trigger2D::Trigger2D(Vector2 position, Vector2 triggerOrigin, float radius)
{
    InitBody(position);

    // Create box shape
    b2CircleShape *shape = new b2CircleShape();
    shape->m_p = b2Vec2(triggerOrigin.x, triggerOrigin.y);
    shape->m_radius = radius;

    // Create sensor (trigger) fixture
    b2FixtureDef *fix = new b2FixtureDef();
    fix->shape = shape;
    fix->isSensor = true;
    AddFixtureToBody(fix);
}

Trigger2D::Trigger2D(Vector2 position, b2Shape *triggerShape)
{
    InitBody(position);
    b2FixtureDef *fix = new b2FixtureDef();
}

void Trigger2D::InitBody(Vector2 position)
{
    // Create new body definition
    bodyDef = new b2BodyDef();
    bodyDef->type = b2_kinematicBody;
    bodyDef->position = b2Vec2(position.x, position.y);
}

void Trigger2D::AddFixtureToBody(b2FixtureDef *newFixture)
{
    if (physicsBody != nullptr)
    {
        b2Fixture *fixture = physicsBody->CreateFixture(newFixture);
    }
    else
    {
        tempFixtures.push_back(newFixture);
    }
}

void Trigger2D::OnRegister(Scene *scene)
{

    b2BodyUserData data = b2BodyUserData();
    data.pointer = (uintptr_t)(new PhysicsEntityData(entityID, name));
    bodyDef->userData = data;
    
    // Don't simulate the trigger until added to the scene
    physicsBody = scene->GetPhysicsWorld()->CreateBody(bodyDef);

    addedToPhysicsWorld = true;
    currentScene = scene;

    
    // Add 'cached' fixtures to the body now that it has been initialized
    for (b2FixtureDef *fix : tempFixtures)
    {
        AddFixtureToBody(fix);

        // Fixtures are *all* created internally via helper functions,
        //  so we can assume they're safe to delete without effecting other entities

        delete fix->shape;
        fix->shape = nullptr;

        delete fix;
        fix = nullptr;
    }

    // Clear the list of cached fixture definitions
    tempFixtures.clear();
}

bool Trigger2D::IsBodyInArea(std::string name)
{
    return bodiesInTrigger.find(name) != bodiesInTrigger.end();
}

#pragma region Contact Functions

void Trigger2D::BeginContact(b2Contact *contact)
{
    b2Body *bodyA = contact->GetFixtureA()->GetBody();
    b2Body *bodyB = contact->GetFixtureB()->GetBody();

    if (bodyA != this->physicsBody && bodyB != this->physicsBody)
    {
        return;
    }

    // Determine which body is the one that collided with this trigger
    b2Body *otherBody = bodyA == this->physicsBody ? bodyB : bodyA;
    PhysicsEntityData *customData = (PhysicsEntityData *)(otherBody->GetUserData().pointer);

    // Add to the bodies in the trigger area
    if (customData != nullptr && bodiesInTrigger.find(customData->entityName) == bodiesInTrigger.end())
    {
        if (customData->entityName == "mixer")
        {
            Debug::Log("Mixer entered: " + name);
        }
        bodiesInTrigger.insert({customData->entityName, otherBody});
    }

    SendEvent("bodyEntered");
}

void Trigger2D::EndContact(b2Contact *contact)
{
    b2Body *bodyA = contact->GetFixtureA()->GetBody();
    b2Body *bodyB = contact->GetFixtureB()->GetBody();

    // Figure out which body is the one that collided with this trigger
    b2Body *otherBody = bodyA == this->physicsBody ? bodyB : bodyA;
    PhysicsEntityData *customData = (PhysicsEntityData *)(otherBody->GetUserData().pointer);

    if (customData != nullptr && bodiesInTrigger.find(customData->entityName) != bodiesInTrigger.end())
    {
        if (customData->entityName == "mixer")
        {
            Debug::Log("Mixer left: " + name);
        }

        bodiesInTrigger.erase(customData->entityName);
    }

    SendEvent("bodyExited");
}

#pragma endregion