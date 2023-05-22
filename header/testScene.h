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
        currentCamera->zoom = 2.0f;
        currentCamera->rotation = 0.0f;

        Vector2 screenSize = Game::instance().GetScreenSize();
        currentCamera->offset = {screenSize.x/2.0f, screenSize.y/2.0f};

        ambientColor = WHITE;
        Light2D* newLight = new Light2D(50.0, 1.0, YELLOW);
        newLight->SetName("testLight");
        newLight->transform.MoveGlobal({10,30.0});
        RegisterEntity(newLight);

        Light2D* testLight2 = new Light2D(50.0, 1.0, RED);
        testLight2->SetName("testLight2");
        testLight2->transform.MoveGlobal({-50,0});
        RegisterEntity(testLight2);


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
        Texture2D spriteRun = tmInstance.GetTexture("res/anim_test.png");
        Texture2D spriteIdle = tmInstance.GetTexture("res/idle_test.png");
        Texture2D beuh = tmInstance.GetTexture("res/beuh.png");

        AnimatedSpriteEntity* testSprite = new AnimatedSpriteEntity(
            "testSprite",
            {25,18}, 
            {50,37},
            spriteRun,
            6,
            6,
            8
            );

        SpriteEntity* testSpr = new SpriteEntity(beuh, {212,180}, {106,90});
        testSpr->transform.MoveGlobal({10,10});
        testSpr->transform.Scale((Vector2){0.2f, 0.2f});


        SpriteAnimation* idleAnim = new SpriteAnimation(spriteIdle, 4, 6, 4, {25,18}, {50,37});

        testSprite->AddAnimation("idle", idleAnim);

        AnimatedSpriteEntity* testChild = new AnimatedSpriteEntity("testChild", {24,24}, {48,48}, spriteIdle);
        testChild->transform.MoveGlobal({20,0});


        TileMap* tm = new TileMap();
        tm->LoadDataTMX("res/tilemap/map.tmx");
        RegisterEntity(tm);

        AnimatedSpriteEntity* animTest = new AnimatedSpriteEntity(
            "animTest",
            {25,18}, 
            {50,37},
            spriteRun,
            6,
            6,
            20
            );

        testChild->transform.scale = (Vector2){0.5,0.5};
        animTest->transform.scale = (Vector2){1,1};

        //testChild->MoveGlobal({0,50});
        //testChild->SetPosition({100,0});
        testSprite->AddChild(testChild);
        //testSprite->AddChild(testLight2);

        animTest->transform.scale = (Vector2){5,5};
       // animTest->MoveGlobal({screenWidth/4.0f ,screenHeight/4.0f });
        //testSprite->MoveGlobal({screenWidth/2.0f ,screenHeight/2.0f });
        //testSprite->RotateDeg(45);

        testSprite->ySortOffset = 10;

        RegisterEntity(testSprite);
        //RegisterEntity(testSpr);
        //RegisterEntity(animTest);
        
        Entity2D* t = GetEntity("testSprite/testChild");
        std::string path = t->GetPath();
        t = GetEntity(path);
        std::string name = t->GetName();
    };

    void Update(float deltaTime)
    {
        Scene::Update(deltaTime);

        InputManager input = InputManager::instance();

        AnimatedSpriteEntity* testSprite = (AnimatedSpriteEntity*)FindEntityByName("testSprite");
        //AnimatedSpriteEntity* animTest = (AnimatedSpriteEntity*)GetEntity("animTest");
        
        if (input.IsActionDown("right"))
        {
            testSprite->transform.MoveLocal({100 * GetFrameTime(),0});
            testSprite->ChangeAnimation("default");
            testSprite->SetFlipped(false, false);
            //if(animTest != nullptr)
            //{
            //     animTest->SetFlipped(false, false);
            //}
           
        }
        else if(input.IsActionDown("left"))
        {
            testSprite->transform.MoveLocal({-100 * GetFrameTime(),0});
            testSprite->ChangeAnimation("default");
            testSprite->SetFlipped(true, false);
            //if(animTest != nullptr)
            //{
            //    animTest->SetFlipped(true, false);
            //    //UnRegisterEntity("animTest");
            //}
        }
        else
        {
            testSprite->ChangeAnimation("idle");
            //testSprite->SetFlipped(false, false);
            //if(animTest != nullptr)
            //{
            //    animTest->SetFlipped(false, false);
            //}
        }
        

        if(input.IsActionDown("up"))
        {
            testSprite->transform.MoveLocal({0,-50 * GetFrameTime()});
        }
        if(input.IsActionDown("down"))
        {
            testSprite->transform.MoveLocal({0, 50 * GetFrameTime()});
        }
        if(input.IsActionDown("rotCW"))
        {
            testSprite->transform.RotateDegrees(30 * GetFrameTime());
        }
        if(input.IsActionDown("rotCCW"))
        {
            testSprite->transform.RotateDegrees(-30 * GetFrameTime());
        }
        
        currentCamera->target = testSprite->GetGlobalPosition();

    }

    

};
#endif // __TESTSCENE_H__