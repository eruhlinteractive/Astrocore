#ifndef __TESTSCENE_H__
#define __TESTSCENE_H__

#include "../AstroCore/astrocore.h"

using namespace Astrolib;

class TestScene : public Scene
{
public:
    TestScene() { LoadScene(); };

    void LoadScene()
    {
        CameraEntity *pixelCamera = new CameraEntity();
        currentCamera = pixelCamera;

        RegisterEntity(pixelCamera);

        Vector2 screenSize = Game::instance().GetScreenSize();
        pixelCamera->SetRenderDimensions(320, 180);

        ambientColor = WHITE;
        Light2D *newLight = new Light2D(50.0, 1.0, YELLOW);
        newLight->SetName("testLight");
        newLight->transform.MoveGlobal({10, 30.0});
        RegisterEntity(newLight);

        Light2D *testLight2 = new Light2D(50.0, 1.0, RED);
        testLight2->SetName("testLight2");
        testLight2->transform.MoveGlobal({-50, 0});
        RegisterEntity(testLight2);

        // Get reference to input instance
        InputManager input = InputManager::instance();

        // Init testing input map
        std::shared_ptr<InputAction> right = std::make_shared<InputAction>("right", KEY_D);
        std::shared_ptr<InputAction> left = std::make_shared<InputAction>("left", KEY_A);
        std::shared_ptr<InputAction> up = std::make_shared<InputAction>("up", KEY_W);
        std::shared_ptr<InputAction> down = std::make_shared<InputAction>("down", KEY_S);

        std::shared_ptr<InputAction> rotCW = std::make_shared<InputAction>("rotCW", KEY_E);
        std::shared_ptr<InputAction> rotCCW = std::make_shared<InputAction>("rotCCW", KEY_Q);
        std::shared_ptr<InputAction> zoomIn = std::make_shared<InputAction>("zoomIn", KEY_K);
        std::shared_ptr<InputAction> zoomOut = std::make_shared<InputAction>("zoomOut", KEY_L);

        input.AddInputAction(right);
        input.AddInputAction(left);
        input.AddInputAction(up);
        input.AddInputAction(down);
        input.AddInputAction(rotCW);
        input.AddInputAction(rotCCW);
        input.AddInputAction(zoomIn);
        input.AddInputAction(zoomOut);

        TextureManager tmInstance = TextureManager::instance();
        Texture2D spriteRun = tmInstance.GetTexture("res/character_walk.png");
        Texture2D spriteIdle = tmInstance.GetTexture("res/idle_test.png");
        Texture2D beuh = tmInstance.GetTexture("res/beuh.png");

        // Create test physics entity
        PhysicsEntity* testPhysicsEntity = new PhysicsEntity(DYNAMIC, {10,50});
        testPhysicsEntity->SetName("physicsTest");
        //testPhysicsEntity->GetPhysicsBody()->SetTransform({-10,0},0.0f);

        SpriteEntity* beuhEntity = new SpriteEntity(beuh, {212,180}, {106, 90});
        beuhEntity->SetName("beuh");
        beuhEntity->transform.Scale({0.2,0.2});
        testPhysicsEntity->CreateRectangleCollider({0,0}, {212 * 0.2, 180 * 0.2});
        //testPhysicsEntity->CreateCircleCollider({0,0}, 50.0);
        testPhysicsEntity->AddChild(beuhEntity);
        //testPhysicsEntity->transform.RotateDegrees(45);

        RegisterEntity(testPhysicsEntity);
        testPhysicsEntity->drawLayer = 100;

        SpriteAnimation *idleAnim = new SpriteAnimation(spriteIdle, 4, 6, 4, {8, 8}, {16, 16});
        SpriteAnimation *runHorizAnim = new SpriteAnimation(spriteRun, 4, 6, 4, {0, 48}, {8, 8}, {16, 16});
        SpriteAnimation *runUpAnim = new SpriteAnimation(spriteRun, 4, 6, 4, {0, 32}, {8, 8}, {16, 16});
        SpriteAnimation *runDownAnim = new SpriteAnimation(spriteRun, 4, 6, 4, {0, 0}, {8, 8}, {16, 16});

        AnimatedSpriteEntity *testAnimSprite = new AnimatedSpriteEntity("testSprite", idleAnim, "idle");
        testAnimSprite->AddAnimation("runHoriz", runHorizAnim);
        testAnimSprite->AddAnimation("runUp", runUpAnim);
        testAnimSprite->AddAnimation("runDown", runDownAnim);

        SpriteEntity *testSpr = new SpriteEntity(beuh, {212, 180}, {106, 90});
        testSpr->transform.MoveGlobal({10, 10});

        PhysicsEntity* playerCollider = new PhysicsEntity(KINEMATIC, {0,0});
        playerCollider->drawLayer = 100;
        playerCollider->SetName("playerCollider");
        playerCollider->CreateRectangleCollider({0,0}, {16,16});
        testAnimSprite->AddChild(playerCollider);
        // testSpr->transform.Scale((Vector2){0.5f, 0.5f});

        //AnimatedSpriteEntity *testChild = new AnimatedSpriteEntity("testChild", {24, 24}, {48, 48}, spriteIdle);
        //testChild->transform.MoveGlobal({20, 0});

        TileMap *tm = new TileMap();
        tm->SetName("tileMap");
        tm->LoadDataTMX("res/tilemap/map.tmx");
        RegisterEntity(tm);
        // tm->SetDrawLayerForMapLayer(1, 100);

        //AnimatedSpriteEntity *animTest = new AnimatedSpriteEntity(
        //    "animTest",
        //    {25, 18},
        //    {50, 37},
        //    spriteRun,
        //    6,
        //    6,
        //    20);
//
        //testChild->transform.scale = (Vector2){0.5, 0.5};
        // animTest->transform.scale = (Vector2){1,1};

        // testChild->MoveGlobal({0,50});
        // testChild->SetPosition({100,0});
        //testAnSprite->AddChild(testChild);
        // testSprite->AddChild(testLight2);

        //animTest->transform.scale = (Vector2){5, 5};
        // animTest->MoveGlobal({screenWidth/4.0f ,screenHeight/4.0f });
        // testSprite->MoveGlobal({screenWidth/2.0f ,screenHeight/2.0f });
        // testSprite->RotateDeg(45);

        //testSprite->ySortOffset = 10;

        RegisterEntity(testAnimSprite);
        // RegisterEntity(testSpr);
        // RegisterEntity(animTest);

        //Entity2D *t = GetEntity("testSprite/testChild");
        //std::string path = t->GetPath();
        //t = GetEntity(path);
        //std::string name = t->GetName();
        //testPhysicsEntity->ApplyImpulse({10000,10000});
    };

    void Update(float deltaTime)
    {

        Scene::Update(deltaTime);

        AnimatedSpriteEntity *testSprite = (AnimatedSpriteEntity *)FindEntityByName("testSprite");
        InputManager input = InputManager::instance();
        PhysicsEntity* physicsTest = (PhysicsEntity*)FindEntityByName("physicsTest");
        PhysicsEntity* playerCollider = (PhysicsEntity*)FindEntityByName("playerCollider");
        TileMap *tm = (TileMap *)FindEntityByName("tileMap");
        //physicsTest->ApplyTorque(10000.0f);

        if (input.IsActionDown("right"))

        {
            testSprite->transform.MoveLocal({50 * deltaTime, 0});
            testSprite->ChangeAnimation("runHoriz");
            testSprite->SetFlipped(false, false);
            // if(animTest != nullptr)
            //{
            //      animTest->SetFlipped(false, false);
            // }
        }
        else if (input.IsActionDown("left"))
        {
            testSprite->transform.MoveLocal({-50 * deltaTime, 0});
            testSprite->ChangeAnimation("runHoriz");
            testSprite->SetFlipped(true, false);
            // if(animTest != nullptr)
            //{
            //     animTest->SetFlipped(true, false);
            //     //UnRegisterEntity("animTest");
            // }
        }
        else if (input.IsActionDown("up"))
        {
            testSprite->transform.MoveLocal({0, -50.0f * deltaTime});
           
            
            testSprite->SetFlipped(false, false);
            testSprite->ChangeAnimation("runUp");
        }
        else if (input.IsActionDown("down"))
        {
            testSprite->transform.MoveLocal({0, 50.0f * deltaTime});
            //playerCollider->AddForce({0,-10});
            testSprite->SetFlipped(false, false);
            testSprite->ChangeAnimation("runDown");
        }
        else
        {
            testSprite->ChangeAnimation("idle");
            // testSprite->SetFlipped(false, false);
            // if(animTest != nullptr)
            //{
            //     animTest->SetFlipped(false, false);
            // }
        }

        

        if (input.IsActionDown("zoomIn"))
        {
            currentCamera->zoom += 1 * deltaTime;
        }
        else if (input.IsActionDown("zoomOut"))
        {
            currentCamera->zoom -= 1 * deltaTime;
        }
        
        if (input.IsActionDown("rotCW"))
        {
            testSprite->transform.RotateDegrees(30 * deltaTime);
        }
        if (input.IsActionDown("rotCCW"))
        {
            testSprite->transform.RotateDegrees(-30 * deltaTime);
        }

        
        //std::cout << std::to_string(targetPos.x) + "," +  std::to_string(targetPos.y) << endl;
        //currentCamera->target = targetPos;
    }

    void LateUpdate(float deltaTime)
    {
        AnimatedSpriteEntity *testSprite = (AnimatedSpriteEntity *)FindEntityByName("testSprite");
        Vector2 targetPos = testSprite->GetGlobalPosition();
        float x = cos(GetTime());
        float y = sin(GetTime());
        float targetX = lerp(currentCamera->target.x, targetPos.x, deltaTime * 80.0);
        float targetY = lerp(currentCamera->target.y, targetPos.y, deltaTime * 80.0);
        //currentCamera->target = targetPos;

        
        currentCamera->target = {targetX, targetY};
        //currentCamera->target.x += deltaTime * 30.0f;
        //currentCamera->target.y += deltaTime * 40.0f;
        //currentCamera->LateUpdate(deltaTime);
        Scene::LateUpdate(deltaTime);
    }

    //void FixedUpdate(float deltaTime)
    //{
    //    // TODO: Add class code here
    //    
    //}

    // Algebraically simplified algorithm
    static float lerp(float a, float b, float f)
    {
        return a + f * (b - a);
    }
};
#endif // __TESTSCENE_H__