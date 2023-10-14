#include "scene.h"
#include <iostream>
using namespace Astrocore;

Scene::Scene()
{
    entities = std::map<std::string, Entity2D *>();
    entityIDMap = std::map<int, Entity2D *>();
    physicsWorld = new b2World(b2Vec2_zero);
    colTracker = CollisionTracker();
    physicsWorld->SetContactListener(&colTracker);
    currentCamera = nullptr;
    sceneResources = std::map<std::string, void *>();

    // screenSpaceLightMap = LoadRenderTexture();
    screenSpaceLightMap = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
    ambientColor = WHITE;

    root = new Entity2D(this->sceneName + "_root");
    // LoadScene();
}

CameraEntityBase *Scene::GetCurrentCamera()
{
    return currentCamera;
}

void Scene::OnNotify(const Signaler *signaler, std::string eventName)
{
    Entity2D *entity = (Entity2D *)signaler;

    // Callback to remove entity from the scene when removed
    if (eventName == "entityDeleted")
    {
        UnRegisterEntity(entity->GetName());
    }

    // Notify scene that the screen has been resized
    if (eventName == "windowResized")
    {
        SendEvent("onWindowResized");
    }
}

bool Scene::RegisterEntity(Entity2D *entity)
{

    // Check if we already have the entity registered
    int entityID = entity->GetEntityID();
    bool hasEntity = entityIDMap.find(entityID) != entityIDMap.end();

    // Already registered, back out
    if(hasEntity)
    {
        return false;
    }

    // int id = entity->GetEntityId();

    // Tell entity that it was registered
    entity->OnRegister(&(*this));

    // Add as a child of the root
    // root->AddChild(entity);

    if (entity->GetChildCount() > 0)
    {
        std::vector<Entity2D *> children = entity->GetChildren();

        for (Entity2D *child : children)
        {
            RegisterEntity(child);
        }
    }

    // Insert new reference
    if (entityIDMap.find(entityID) == entityIDMap.end())
    {
        std::string name = entity->GetName();

        // Add to maps
        entities.insert(std::pair{name, entity});
        entityIDMap.insert(std::pair(entity->GetEntityID(), entity));

        // Add to specialized lists
        if (entity->GetType() == SPRITE || entity->GetType() == TILEMAP)
        {
            drawableEntities.insert(std::pair{name, entity});
        }

        if (entity->GetType() == LIGHT)
        {
            lights.insert(std::pair{name, (Light2D *)entity});
        }
        if (entity->GetType() == PHYSICAL)
        {
            colTracker.AddTrackedEntity(entity->GetEntityID(), (CollisionEntity *)entity);
        }
        if (entity->GetType() == CAMERA)
        {
            AddObserver(entity, "onWindowResized");
        }

        // Register to be notified when an entity is deleted
        entity->AddObserver(this, "entityDeleted");
        return true;
    }


    return false;
}

Scene::~Scene()
{
    if (!entities.empty())
    {
        auto last = *entities.begin();
        std::vector<string> currentEntityNames = std::vector<string>();

        // Cleanup scene memory
        for (auto e : entities)
        {
            currentEntityNames.push_back(e.first);
        }

        // Loop through all of the entity names to delete remaining entities in the scene
        while (!currentEntityNames.empty())
        {
            std::string name = currentEntityNames[currentEntityNames.size() - 1];

            if (entities.find(name) != entities.end())
            {
                Entity2D *val = entities[name];
                Debug::Log("Deleting: " + val->GetName());
                delete val;
                val = nullptr;
            }
            currentEntityNames.pop_back();
        }
    }

    delete root;
    root = nullptr;
    delete physicsWorld;

    UnloadRenderTexture(screenSpaceLightMap);
}

bool Scene::UnRegisterEntity(std::string name)
{

    if (entities.find(name) != entities.end())
    {
        Entity2D* entity = entities[name];

        // Remove from render queue
        if (drawableEntities.find(name) != drawableEntities.end())
        {
            drawableEntities.erase(name);
        }

        // Remove from scene maps
        entityIDMap.erase(entity->GetEntityID());
        entities.erase(name);
        return true;
    }

    return false;
}

Entity2D *Scene::GetEntity(std::string path)
{
    // Split string by '/' token (oh lord help me)
    std::vector<std::string> split;

    std::string s = "";
    for (char c : path)
    {
        if (c != '/')
        {
            s += c;
        }
        else
        {
            if (s != "")
            {
                split.push_back(s);
                s = "";
            }
        }
    }
    if (s != "")
    {
        split.push_back(s);
    }

    // Traverse tree to locate Entity

    // Top level entity isn't found
    if (entities.find(split[0]) == entities.end())
    {
        return nullptr;
    }

    // Iterate through path to find entity
    Entity2D *current = entities[split[0]];

    for (int i = 1; i < split.size(); i++)
    {
        current = current->GetChild(split[i]);
        if (current == nullptr)
        {
            return nullptr;
        }
    }
    // Navigate through children
    return current;
}

Entity2D* Scene::GetEntityById(int entityID)
{
    if(entityIDMap.find(entityID) != entityIDMap.end())
    {
        return entityIDMap[entityID];
    }

    // Something has gone terribly wrong
    return nullptr;
}

Entity2D *Scene::FindEntityByName(std::string name)
{
    if (entities.find(name) != entities.end())
    {
        return entities[name];
    }
    return nullptr;
}

void Scene::RegisterResource(string name, void *resource)
{
    // Only register if it's not already registered
    if (sceneResources.find(name) == sceneResources.end())
    {
        sceneResources.insert(std::pair{name, resource});
    }
    else
    {
        Debug::Assert(sceneResources.find(name) != sceneResources.end(), "Resource already exists in the scene!");
    }
}

bool Scene::IsOnScreen(Rectangle entityRect)
{
    Vector2 renderSize = GetCurrentCamera()->GetRenderResolution();
    Vector2 screenSpaceCoords = GetWorldToScreen2D({entityRect.x, entityRect.y}, *(currentCamera->GetCamera()));
    Rectangle tileRect = (Rectangle){
        screenSpaceCoords.x,
        screenSpaceCoords.y,
        entityRect.width,
        entityRect.height};

    // Debug::Log(std::to_string(tileRect.x));
    //  Vector2 ss = GetScreenToWorld2D({tileRect.x, tileRect.y}, *camera);
    //  Rectangle debugRect = (Rectangle){ss.x, ss.y, tileInfo->imageSize.x, tileInfo->imageSize.y};

    bool isOnScreen =
        tileRect.x + tileRect.width > 0 &&
        tileRect.x < renderSize.x &&
        tileRect.y + tileRect.height > 0 &&
        tileRect.y < renderSize.y;

    return isOnScreen;
}

void *Scene::GetResource(std::string name)
{
    if (HasResource(name))
    {
        return sceneResources[name];
    }
    return nullptr;
}

void Scene::SetCurrentCamera(CameraEntityBase *newCamera)
{
    // Attempt to register
    RegisterEntity(newCamera);

    currentCamera = newCamera;
}

bool Scene::HasResource(std::string name)
{
    return sceneResources.find(name) != sceneResources.end();
}

float Scene::GetPhysicsFraction()
{
    return physicsStepFraction;
}

Vector2 Scene::GetWorldRenderSize()
{
    if (currentCamera == nullptr)
    {
        return (Vector2){(float)GetScreenWidth(), (float)GetScreenHeight()};
    }

    return currentCamera->GetRenderResolution();
}

void Scene::Update(float deltaTime)
{
    // Don't process if the scene is currently paused
    // TODO: Add ability to pause individual nodes
    if (isPaused)
    {
        return;
    }

    for (auto e : entities)
    {
        e.second->Update(deltaTime);
    }
}

void Scene::LateUpdate(float deltaTime)
{
    for (auto e : entities)
    {
        e.second->LateUpdate(deltaTime);
    }
}

void Scene::FixedUpdate(float deltaTime)
{
    physicsWorld->Step(deltaTime, 8, 3);
    physicsWorld->ClearForces();
    for (std::pair<std::string, Entity2D *> p : entities)
    {
        p.second->FixedUpdate(deltaTime);
    }
}

void Scene::Draw(float deltaTime)
{
    std::vector<Entity2D *> pairs;

    // Screen border buffer to prevent pop-ins
    float rimBuffer = 100.0f * currentCamera->zoom;

    // Create a list of all entities in the scene
    for (std::pair<std::string, Entity2D *> p : drawableEntities)
    {
        Vector2 screenPos = GetWorldToScreen2D(p.second->GetGlobalPosition(), *currentCamera->GetCamera());

        // Don't render if we aren't visible
        if (!p.second->IsVisible())
        {
            continue;
        }

        // AABB test the rect to see if it's on screen and add if it is AND is visible
        if (IsOnScreen(p.second->GetBoundRect()) && p.second->GetType() != TILEMAP)
        {
            pairs.push_back((Entity2D *)p.second);
        }

        // Append layers of a tilemap to be sorted in order
        else if (p.second->GetType() == TILEMAP)
        {
            for (auto layer : ((TileMap *)p.second)->GetTileLayers())
            {
                pairs.push_back(layer);
            }
        }
    }

    // Sort pair list
    if (ySortEnabled)
    {
        // Sort by draw layer
        std::sort(pairs.begin(), pairs.end(), SortByLayerIndexWithYSort);
    }
    else
    {
        // Sort by draw layer
        std::sort(pairs.begin(), pairs.end(), SortByLayerIndex);
    }

    currentCamera->BeginDrawing();
    ClearBackground(WHITE);
    if (Debug::IsDebugFlagSet(DRAW_WORLD_GRID))
    {
        Debug::DrawWorldGrid(100, 100, currentCamera->GetGlobalPosition());
    }

    for (Entity2D *e : pairs)
    {
        e->Draw(deltaTime, currentCamera->GetCamera());
    }

    if (Debug::IsDebugFlagSet(DRAW_PHYSICS_BOUNDS))
    {
        GetPhysicsWorld()->DebugDraw();
    }
    
    Debug::DrawDebugShapes();
    currentCamera->EndDrawing();

    BeginDrawing();
    currentCamera->Draw(deltaTime, currentCamera->GetCamera());
    // EndMode2D();

    // if (currentCamera->GetType() == PIXELCAMERA)
    //{
    //     EndTextureMode();
    // }
    //  Render lights
    //  Based on https://slembcke.github.io/2D-Lighting-Overview

    /*
    // Clear background
    BeginTextureMode(screenSpaceLightMap);
    ClearBackground(ambientColor);

    BeginBlendMode(BLEND_ADDITIVE);
    // Draw each light to buffer texture
    for (std::pair<std::string, Light2D *> lightPair : lights)
    {
        Light2D *light = lightPair.second;
        Vector2 pos = light->GetGlobalPosition();
        Vector2 screenPos = GetWorldToScreen2D(light->GetGlobalPosition(), *currentCamera->GetCamera());
        DrawCircleGradient(screenPos.x, screenPos.y, light->distance * currentCamera->zoom, light->color, ColorAlpha(light->color, 0.0));
    }
    EndBlendMode();
    EndTextureMode();



    Rectangle lightSrc, lightDest;

    lightSrc = (Rectangle){0, 0, (float)GetScreenWidth(), (float)-GetScreenHeight()};
    lightDest = (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()};
    */
    // if (currentCamera->GetType() == PIXELCAMERA)
    //{
    //
    //     BeginDrawing();
    //     //ClearBackground(RAYWHITE);
    //     currentCamera->Draw(deltaTime, currentCamera->GetCamera());
    // }

    /*
    BeginBlendMode(BLEND_MULTIPLIED);
    DrawTexturePro(screenSpaceLightMap.texture, lightSrc, lightDest, (Vector2){0, 0}, 0, WHITE);
    EndBlendMode();
    */

    // std::string val = std::to_string(currentCamera->zoom);
    // DrawText(val.c_str(), 10, 30, 20, DARKGREEN);
    DrawFPS(10, 10);

   
    EndDrawing();
}
