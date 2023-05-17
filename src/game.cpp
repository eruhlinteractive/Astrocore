#include "game.h"

void Game::InitGame(int screenWidth, int screenHeight)
{

    worldCamera = new Camera2D();
    

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


    Texture2D spriteRun = LoadTexture("../src/res/anim_test.png");
    Texture2D spriteIdle = LoadTexture("../src/res/idle_test.png");

    Texture2D beuh = LoadTexture("../src/res/beuh.png");

    testSprite = new AnimatedSpriteEntity(
        {25,18}, 
        {50,37},
        spriteRun,
        6,
        6,
        8
        );

    testSpr = new SpriteEntity(beuh, {212,180}, {0,0});
    testSpr->MoveGlobal({10,10});

    SpriteAnimation* idleAnim = new SpriteAnimation(spriteIdle, 4, 6, 4, {25,18}, {50,37});

    testSprite->AddAnimation("idle", idleAnim);

    AnimatedSpriteEntity* testChild = new AnimatedSpriteEntity({24,24}, {48,48}, spriteIdle);

    animTest = new AnimatedSpriteEntity(
        {25,18}, 
        {50,37},
        spriteRun,
        6,
        6,
        20
        );

    testChild->SetScale({0.5,0.5});
    animTest->SetScale({1,1});

    //testChild->MoveGlobal({screenWidth/2.0,screenHeight/2.0});
    //testChild->SetPosition({100,0});
    testSprite->AddChild(testChild);
    
    animTest->SetScale({5,5});
   // animTest->MoveGlobal({screenWidth/4.0f ,screenHeight/4.0f });
    //testSprite->MoveGlobal({screenWidth/2.0f ,screenHeight/2.0f });
    testSprite->RotateDeg(45);
    //worldCamera->target = testSprite->GetGlobalPosition();

}


void Game::OnExit()
{

}

void Game::Update(float deltaTime)
{
    // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------
    
        InputManager input = InputManager::instance();

        if (input.IsActionDown("right"))
        {
            testSprite->MoveLocal({100 * GetFrameTime(),0});
            testSprite->ChangeAnimation("default");
            testSprite->SetFlipped(false, false);
            animTest->SetFlipped(false, false);
        }
        else if(input.IsActionDown("left"))
        {
            testSprite->MoveLocal({-100 * GetFrameTime(),0});
            testSprite->ChangeAnimation("default");
            testSprite->SetFlipped(true, false);
            animTest->SetFlipped(true, false);
        }
        else
        {
            testSprite->ChangeAnimation("idle");
            //testSprite->SetFlipped(false, false);
            animTest->SetFlipped(false, false);
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
            testSprite->RotateDeg(1);
        }
        if(input.IsActionDown("rotCCW"))
        {
            testSprite->RotateDeg(-1);
        }
    worldCamera->target = testSprite->GetGlobalPosition();
        
    
}

#pragma region Drawing Logic
void Game::Draw(float deltaTime)
{

    // Draw world
    //BeginMode2D(*worldCamera);

    DrawWorld();
   //animTest->Draw(1.0/60.0);
    //EndMode2D();
    DrawUI();   
}

void Game::DrawWorld()
{
    /// ---- World Drawing Logic Here ----
    testSprite->Draw(1.0f/60.0f);
    
    //testChild->Draw(1.0f/60.0f);
    animTest->Draw(1.0/60.0);
}

void Game::DrawUI()
{
    DrawFPS(10,10);
}
#pragma endregion