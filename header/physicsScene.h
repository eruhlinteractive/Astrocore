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
        // currentCamera->zoom = 9.0;
        currentCamera->SetRenderDimensions(320, 180);
        Game::GetPhysicsWorld()->SetGravity({0, 9.8});

        PhysicsEntity *floor = new PhysicsEntity(STATIC, {-2, 0});
        floor->CreateRectangleCollider({0, 0}, {720, 20},0,1.0,1.0,0.0);
        RegisterEntity(floor);
        // PhysicsEntity *box1 = new PhysicsEntity(DYNAMIC, {-2, -50});
        // box1->CreateRectangleCollider({0, 0}, {5, 5});
        // RegisterEntity(box1);

        // PhysicsEntity *box2 = new PhysicsEntity(DYNAMIC, {-5, -60});
        // box2->CreateRectangleCollider({0, 0}, {5, 5});
        // RegisterEntity(box2);

        for (int i = 0; i < 10; i++)
        {
            PhysicsEntity *box = new PhysicsEntity(DYNAMIC, {-i * 2.0, -50 + (-i * 10)});
            box->CreateRectangleCollider({0, 0}, {5, 6});
            RegisterEntity(box);
        }
    }
};

#endif // __PHYSICSSCENE_H__