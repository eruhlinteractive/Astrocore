#include "../header/scene.h"
#include <iostream>
// For function timing
#include <chrono>
using namespace std::chrono;

using namespace Astrolib;

Scene::Scene()
{
    entities = std::map<std::string, Entity2D *>();

    // screenSpaceLightMap = LoadRenderTexture();
    screenSpaceLightMap = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
    ambientColor = WHITE;
    LoadScene();
}

bool Scene::RegisterEntity(Entity2D *entity)
{
    std::string name = entity->GetName();
    // int id = entity->GetEntityId();

    if (entity->GetChildCount() > 0)
    {
        std::vector<Entity2D *> children = entity->GetChildren();

        for (Entity2D *child : children)
        {
            RegisterEntity(child);
        }
    }

    // Insert new reference
    if (entities.find(name) == entities.end())
    {
        entities.insert(std::pair{name, entity});

        // Add to specialized lists
        if (entity->GetType() == SPRITE || entity->GetType() == TILEMAP)
        {
            drawableEntities.insert(std::pair{name, entity});
        }

        else if (entity->GetType() == LIGHT)
        {
            lights.insert(std::pair{name, (Light2D *)entity});
        }

        return true;
    }

    // The entity is not the same as the already registered entity
    else if (entities[name]->GetEntityID() != entity->GetEntityID())
    {
        throw std::runtime_error("Failed to register entity: Name " + name + " already exists in scene!");
        return false;
    }

    return false;
}

Scene::~Scene()
{
    // Cleanup scene memory
    for (auto e : entities)
    {
        delete e.second;
        e.second = nullptr;
    }
    UnloadRenderTexture(screenSpaceLightMap);
}

bool Scene::UnRegisterEntity(std::string name)
{

    if (entities.find(name) != entities.end())
    {
        // Remove from render queue
        if (drawableEntities.find(name) != drawableEntities.end())
        {
            drawableEntities.erase(name);
        }

        delete entities[name];
        entities[name] = nullptr;

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

Entity2D *Scene::FindEntityByName(std::string name)
{
    if (entities.find(name) != entities.end())
    {
        return entities[name];
    }
    return nullptr;
}

void Scene::Update(float deltaTime)
{
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

        // TODO: Update this to a proper AABB test
        if (IsOnScreen(currentCamera->GetCamera(), p.second->GetSpriteRect()) && p.second->GetType() != TILEMAP)
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
            // auto layer = ((TileMap *)p.second)->GetTileLayers()[1];
            // pairs.push_back(layer);
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

    

    if (currentCamera->GetType() == PIXELCAMERA)
    {
        RenderTexture2D text = *(((PixelPerfectCamera2D *)currentCamera)->GetRenderTexture());
        BeginTextureMode(text);
        ClearBackground(RAYWHITE);


        DrawLine(GetRenderWidth() / 2, 0, GetRenderWidth()/2, GetRenderHeight(), GRAY);
        DrawLine(0, GetRenderHeight()/2, GetRenderWidth(), GetRenderHeight()/2, GRAY);
        //ClearBackground(RAYWHITE);
    }
    else
    {
        BeginDrawing();
    }
    
    
    
    Camera2D cam = *(currentCamera->GetCamera());
    BeginMode2D(cam);
  
    for (Entity2D *e : pairs)
    {
        e->Draw(deltaTime, &cam );
    }

    EndMode2D();

    if (currentCamera->GetType() == PIXELCAMERA)
    {
        EndTextureMode();
    }
    // Render lights
    // Based on https://slembcke.github.io/2D-Lighting-Overview

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
    if (currentCamera->GetType() == PIXELCAMERA)
    {
        
        BeginDrawing();
        //ClearBackground(RAYWHITE);
        currentCamera->Draw(deltaTime, currentCamera->GetCamera());
    }
    
    /*
    BeginBlendMode(BLEND_MULTIPLIED);
    DrawTexturePro(screenSpaceLightMap.texture, lightSrc, lightDest, (Vector2){0, 0}, 0, WHITE);
    EndBlendMode();
    */
    std::string val = std::to_string(currentCamera->zoom);
    DrawText(val.c_str(), 10, 30, 20, DARKGREEN);
    DrawFPS(10,10); 
   
    EndDrawing();
}
