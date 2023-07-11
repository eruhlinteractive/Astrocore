#ifndef __PHYSICSSCENE_H__
#define __PHYSICSSCENE_H__

#include "../AstroCore/astrocore.h"

using namespace Astrolib;

class PhysicsScene : public Scene
{
public:
    PhysicsScene() { LoadScene(); };
    void LoadScene()
    {
        currentCamera = new CameraEntity();
        RegisterEntity(currentCamera);
        currentCamera->zoom = 9.0;
        Game::GetPhysicsWorld()->SetGravity({0,9.8});

        PhysicsEntity *floor = new PhysicsEntity(STATIC, {-2, 0});
        floor->CreateRectangleCollider({0, 0}, {720, 20});
        RegisterEntity(floor);

        PhysicsEntity *box1 = new PhysicsEntity(DYNAMIC, {-2, -50});
        box1->CreateRectangleCollider({0, 0}, {5, 5});
        RegisterEntity(box1);

        PhysicsEntity *box2 = new PhysicsEntity(DYNAMIC, {-7, -60});
        box2->CreateRectangleCollider({0, 0}, {5, 5});
        RegisterEntity(box2);
    }

};

#endif // __PHYSICSSCENE_H__