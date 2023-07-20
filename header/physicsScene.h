#ifndef __PHYSICSSCENE_H__
#define __PHYSICSSCENE_H__

#include "../AstroCore/astrocore.h"

using namespace Astrolib;

class PhysicsScene : public Scene
{
public:
    PhysicsScene()
    {
        LoadScene();
    };
    void LoadScene()
    {
        currentCamera = new CameraEntity();
        RegisterEntity(currentCamera);
        currentCamera->zoom = 2.0;
        currentCamera->target.y -= 30;
        currentCamera->SetRenderDimensions(320, 180);
        GetPhysicsWorld()->SetGravity({0.0, 9.8});

        //PhysicsEntity *floor = new PhysicsEntity(STATIC, {-2, 0});
        //floor->CreateRectangleCollider({0, 0}, {720, 20}, 0, 0.0, 1.0, 0.0);
        //RegisterEntity(floor);

        // PhysicsEntity *box1 = new PhysicsEntity(DYNAMIC, {-2, -50});
        // box1->CreateRectangleCollider({0, 0}, {5, 5});
        // RegisterEntity(box1);

        // PhysicsEntity *box2 = new PhysicsEntity(DYNAMIC, {-5, -60});
        // box2->CreateRectangleCollider({0, 0}, {5, 5});
        // RegisterEntity(box2);
        entities = std::vector<PhysicsEntity *>();

        // Get reference to input instance
        InputManager input = InputManager::instance();

        // Init testing input map
        std::shared_ptr<InputAction> right = std::make_shared<InputAction>("right", KEY_D);
        std::shared_ptr<InputAction> left = std::make_shared<InputAction>("left", KEY_A);
        std::shared_ptr<InputAction> up = std::make_shared<InputAction>("up", KEY_W);
        std::shared_ptr<InputAction> down = std::make_shared<InputAction>("down", KEY_S);

        std::shared_ptr<InputAction> rotCW = std::make_shared<InputAction>("rotCW", KEY_E);
        std::shared_ptr<InputAction> rotCCW = std::make_shared<InputAction>("rotCCW", KEY_Q);

        input.AddInputAction(right);
        input.AddInputAction(left);
        input.AddInputAction(up);
        input.AddInputAction(down);
        input.AddInputAction(rotCW);
        input.AddInputAction(rotCCW);

        std::shared_ptr<InputAction> addForce = std::shared_ptr<InputAction>(new InputAction("addForce", KEY_SPACE));
        InputManager::instance().AddInputAction(addForce);

        Trigger2D *trigger = new Trigger2D({0, -12.5}, {0, 0}, {720, 10});
        RegisterEntity(trigger);

        // Trigger2D* trigger2 = new Trigger2D({0, -25.5},{0,0}, {720,10});
        // RegisterEntity(trigger2);

        for (int i = 0; i < 12; i++)
        {
            PhysicsEntity *box = new PhysicsEntity(DYNAMIC, {-i * 3.0 + 10, -50 + (-i * 10)});
            box->CreateRectangleCollider({0, 0}, {6.7, 6.7}, 0, 1, 5, 0);

            Texture spriteTexture = TextureManager::instance().GetTexture("res/testSprite.png");
            SpriteEntity *spriteChild = new SpriteEntity(spriteTexture, {48, 48}, {24, 24});
            // spriteChild->transform.MoveGlobal({-4,-4});
            spriteChild->transform.Scale({0.15, 0.15});
            box->AddChild(spriteChild);
            RegisterEntity(box);
            Debug::Log("Created Body: " + std::to_string(i));

            entities.push_back(box);
        }

        PhysicsEntity *box = new PhysicsEntity(DYNAMIC, {0, -13});
        box->SetName("mixer");
        box->CreateRectangleCollider({0, 0}, {6.7, 6.7});

        Texture spriteTexture = TextureManager::instance().GetTexture("res/idle_test.png");
        SpriteEntity *spriteChild = new SpriteEntity(spriteTexture, {16, 16}, {8, 8});
        spriteChild->GetSprite()->spriteSize = (Vector2){16, 16};
        // spriteChild->transform.MoveGlobal({-4,-4});
        spriteChild->transform.Scale({0.5, 0.5});
        box->AddChild(spriteChild);
        RegisterEntity(box);
        box->GetPhysicsBody()->SetSleepingAllowed(false);
        box->GetPhysicsBody()->SetLinearDamping(100.0);
        box->GetPhysicsBody()->SetFixedRotation(true);

        entities.push_back(box);
    }

    void FixedUpdate(float deltaTime) override
    {
        Scene::FixedUpdate(deltaTime);
        PhysicsEntity *mixer = (PhysicsEntity *)FindEntityByName("mixer");
        // currentCamera->target = mixer->GetGlobalPosition();
        //  float x = (float)std::cos(GetTime()) * 10.0f;
        //  Debug::Log(std::to_string(x));
        //  mixer->transform.position.x = x;

        if (InputManager::instance().IsActionPressed("addForce"))
        {
            for (PhysicsEntity *e : entities)
            {
                Debug::Log("AHH");
                e->ApplyImpulse({0, 1000});
            }
        }

        // Get reference to input instance
        InputManager input = InputManager::instance();

        if (input.IsActionDown("up"))
        {
            mixer->AddForce({0.0, 100000});
        }
        else if (input.IsActionDown("down"))
        {
            mixer->AddForce({0.0, -100000});
        }

        if (input.IsActionDown("right"))
        {
            mixer->AddForce({-100000, 0});
        }
        else if (input.IsActionDown("left"))
        {
            mixer->AddForce({100000, 0});
        }

        //std::string pos = std::to_string(WorldRenderedScreenCoords(mixer->GetGlobalPosition()).x) + "," + std::to_string(WorldRenderedScreenCoords(mixer->GetGlobalPosition()).y);
        //Debug::Log(std::to_string(IsOnScreen(mixer->GetBoundRect())));
    }

private:
    std::vector<PhysicsEntity *> entities;
};

#endif // __PHYSICSSCENE_H__