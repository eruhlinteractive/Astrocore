#ifndef __TESTSCENE_H__
#define __TESTSCENE_H__

#include "../AstroCore/astrocore.h"
using namespace Astrolib;

class TestScene : public Scene
{
public:
    TestScene(){LoadScene();};


    void LoadScene()
    {
        currentCamera = new Camera2D();
        currentCamera->zoom = 2.0f;
        currentCamera->rotation = 0.0f;

        Vector2 screenSize = Game::instance().GetScreenSize();
        currentCamera->offset = {screenSize.x/2.0f, screenSize.y/2.0f};



        // Get reference to input instance
        InputManager input = InputManager::instance();

        // Init testing input map
        std::shared_ptr<InputAction> right = std::make_shared<InputAction>("right",KEY_D);
        std::shared_ptr<InputAction> left = std::make_shared<InputAction>("left",KEY_A);
        std::shared_ptr<InputAction> up = std::make_shared<InputAction>("up",KEY_W);
        std::shared_ptr<InputAction> down = std::make_shared<InputAction>("down",KEY_S);

        std::shared_ptr<InputAction> rotCW = std::make_shared<InputAction>("rotCW",KEY_E);
        std::shared_ptr<InputAction> rotCCW = std::make_shared<InputAction>("rotCCW",KEY_Q);
    
        input.AddInputAction(right);
        input.AddInputAction(left);
        input.AddInputAction(up);
        input.AddInputAction(down);
        input.AddInputAction(rotCW);
        input.AddInputAction(rotCCW);

        TextureManager tmInstance = TextureManager::instance();
        Texture2D spriteRun = tmInstance.GetTexture("../src/res/anim_test.png");
        Texture2D spriteIdle = tmInstance.GetTexture("../src/res/idle_test.png");

        Texture2D beuh = tmInstance.GetTexture("../src/res/beuh.png");


        AnimatedSpriteEntity* testSprite = new AnimatedSpriteEntity(
            {25,18}, 
            {50,37},
            spriteRun,
            6,
            6,
            8
            );

        SpriteEntity* testSpr = new SpriteEntity(beuh, {212,180}, {106,90});
        testSpr->MoveGlobal({10,10});
        testSpr->Scale((Vector2){1.0f,1.0f});


        SpriteAnimation* idleAnim = new SpriteAnimation(spriteIdle, 4, 6, 4, {25,18}, {50,37});

        testSprite->AddAnimation("idle", idleAnim);

        AnimatedSpriteEntity* testChild = new AnimatedSpriteEntity({24,24}, {48,48}, spriteIdle);

        AnimatedSpriteEntity* animTest = new AnimatedSpriteEntity(
            {25,18}, 
            {50,37},
            spriteRun,
            6,
            6,
            20
            );

        testChild->SetScale({0.5,0.5});
        animTest->SetScale({1,1});

        //testChild->MoveGlobal({0,50});
        //testChild->SetPosition({100,0});
        testSprite->AddChild(testChild);

        animTest->SetScale({5,5});
       // animTest->MoveGlobal({screenWidth/4.0f ,screenHeight/4.0f });
        //testSprite->MoveGlobal({screenWidth/2.0f ,screenHeight/2.0f });
        //testSprite->RotateDeg(45);

        testSprite->ySortOffset = 10;

        RegisterEntity("Test Sprite Player", testSprite);
        RegisterEntity("Test Sprite Static", testSpr);
        RegisterEntity("Test Sprite Animated", animTest);
       
        // TODO: Figure out how to unload/manage textures
    };

    void Update(float deltaTime)
    {
        Scene::Update(deltaTime);

        InputManager input = InputManager::instance();

        AnimatedSpriteEntity* testSprite = (AnimatedSpriteEntity*)FindEntityByName("Test Sprite Player");
        AnimatedSpriteEntity* animTest = (AnimatedSpriteEntity*)FindEntityByName("Test Sprite Animated");
        
        if (input.IsActionDown("right"))
        {
            testSprite->MoveLocal({100 * GetFrameTime(),0});
            testSprite->ChangeAnimation("default");
            testSprite->SetFlipped(false, false);
            if(animTest != nullptr)
            {
                 animTest->SetFlipped(false, false);
            }
           
        }
        else if(input.IsActionDown("left"))
        {
            testSprite->MoveLocal({-100 * GetFrameTime(),0});
            testSprite->ChangeAnimation("default");
            testSprite->SetFlipped(true, false);
            if(animTest != nullptr)
            {
                animTest->SetFlipped(true, false);
                UnRegisterEntity("Test Sprite Animated");
            }
        }
        else
        {
            testSprite->ChangeAnimation("idle");
            //testSprite->SetFlipped(false, false);
            if(animTest != nullptr)
            {
                animTest->SetFlipped(false, false);
            }
        }
        

        if(input.IsActionDown("up"))
        {
            testSprite->MoveLocal({0,-50 * GetFrameTime()});
        }
        if(input.IsActionDown("down"))
        {
            testSprite->MoveLocal({0, 50 * GetFrameTime()});
        }
        if(input.IsActionDown("rotCW"))
        {
            testSprite->RotateDeg(30 * GetFrameTime());
        }
        if(input.IsActionDown("rotCCW"))
        {
            testSprite->RotateDeg(-30 * GetFrameTime());
        }
        
        currentCamera->target = testSprite->GetGlobalPosition();

    }

    

};
#endif // __TESTSCENE_H__