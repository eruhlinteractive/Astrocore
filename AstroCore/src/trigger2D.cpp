#include "../header/trigger2D.h"

using namespace Astrolib;

Trigger2D::Trigger2D(Vector2 position, Vector2 triggerOrigin, Vector2 triggerSize)
{
    b2BodyDef newDef = b2BodyDef();
    newDef.type = b2_kinematicBody;
    newDef.position = b2Vec2{position.x, position.y};
}

void Trigger2D::InitBody(b2BodyDef bodyDef)
{
}