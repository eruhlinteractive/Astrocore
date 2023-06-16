#include "../header/scene.h"

// For function timing
#include <chrono>
using namespace std::chrono;

using namespace Astrolib;

Scene::Scene()
{
    entities = std::map<std::string, Entity2D*>();
    currentCamera = new Camera2D();
    //screenSpaceLightMap = LoadRenderTexture();
    screenSpaceLightMap = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
    ambientColor = WHITE;
    LoadScene();
}

bool Scene::RegisterEntity(Entity2D* entity)
{
    std::string name = entity->GetName();
    //int id = entity->GetEntityId();

    if(entity->GetChildCount() > 0)
    {
        std::vector<Entity2D*> children = entity->GetChildren();

        for(Entity2D* child : children)
        {
            RegisterEntity(child);
        }
    }

    // Insert new reference
    if(entities.find(name) == entities.end())
    {
        entities.insert(std::pair{name, entity});
        return true;
    }

    // The entity is not the same as the already registered entity
    else if(entities[name]->GetEntityID() != entity->GetEntityID())
    {
        throw std::runtime_error("Failed to register entity: Name " + name + " already exists in scene!");
        return false;
    }

    return false;
}

Scene::~Scene()
{
    // Cleanup scene memory
    for(auto e : entities)
    {
        delete e.second;
        e.second = nullptr;
    }
    
    UnloadRenderTexture(screenSpaceLightMap);
    delete currentCamera;
    currentCamera = nullptr;

}

bool Scene::UnRegisterEntity(std::string name)
{
    if(entities.find(name) != entities.end())
    {
        delete entities[name];
        entities[name] = nullptr;

        entities.erase(name);
        return true;
    }
    
    return false;
}

Entity2D* Scene::GetEntity(std::string path)
{
    // Split string by '/' token (oh lord help me)
    std::vector<std::string> split;

    std::string s = "";
    for(char c : path)
    {
        if(c != '/')
        {
            s += c;
        }
        else
        {   if(s != "")
            {
                split.push_back(s);
                s = "";
            }
        }
    }
    if(s != "")
    {
        split.push_back(s);
    }

    // Traverse tree to locate Entity

    // Top level entity isn't found
    if(entities.find(split[0]) == entities.end())
    {
        return nullptr;
    }

    // Iterate through path to find entity
    Entity2D* current = entities[split[0]];

    for(int i = 1; i < split.size(); i++)
    {
        current = current->GetChild(split[i]);
        if(current == nullptr)
        {
            return nullptr;
        }
    }
    // Navigate through children   
    return current;
}


Entity2D* Scene::FindEntityByName(std::string name)
{
    if(entities.find(name) != entities.end())
    {
        return entities[name];
    }
    return nullptr;
}



void Scene::Update(float deltaTime)
{
    for(auto e : entities)
    {
        e.second->Update(deltaTime);
    }
}


void Scene::Draw(float deltaTime)
{
    std::vector<Entity2D*> pairs;
    std::vector<Light2D*> lights;
    std::vector<Entity2D*> tileMaps;

    // Screen border buffer to prevent pop-ins
    float rimBuffer = 100.0f * currentCamera->zoom;

    // Create a list of all entities in the scene
    for (std::pair<std::string, Entity2D*> p : entities)
    {
        Vector2 screenPos = GetWorldToScreen2D(p.second->GetGlobalPosition(), *currentCamera);

        if(IsOnScreen(screenPos, rimBuffer ) && p.second->drawType != TILEMAP)
        {
            pairs.push_back((Entity2D*)p.second);
        }
        else if (p.second->drawType == TILEMAP)
        {
            tileMaps.push_back((TileMap*)p.second);
        }        
    }

    if(ySortEnabled)
    {
        // Sort by draw layer
        std::sort(pairs.begin(), pairs.end(), SortByLayerIndexWithYSort);
    }
    else
    {
        // Sort by draw layer
        std::sort(pairs.begin(), pairs.end(), SortByLayerIndex);
    }
    
    BeginMode2D(*currentCamera);

    for (Entity2D* m : tileMaps)
    {
        m->Draw(deltaTime);
    }
    // Draw every entity
    for(Entity2D* e : pairs)
    {
        // Don't draw if it's off screen
        if(e->drawType == NORMAL)
        {
            e->Draw(deltaTime);
        }
        else if(e->drawType == LIGHT)
        {
            //lights.push_back((Light2D*)e);
        }
        else if(e->drawType == TILEMAP)
        {
            
            //auto start = high_resolution_clock::now();
            ((TileMap*)e)->Draw(deltaTime);
            //tileMaps.push_back((TileMap*)e);
            //auto end = high_resolution_clock::now();
            //auto length = duration_cast<milliseconds>(end - start);
            //printf("Tilemap rendered in %d milliseconds \n", length.count());
        }
    }

    EndMode2D();
    //BeginMode2D(*currentCamera);

    
    //// Ignore on-screen for tile map
    //for(auto& map: tileMaps)
    //{
    //    map->Draw(deltaTime);
    //}

    //EndMode2D();

    // Render lights
    // Based on https://slembcke.github.io/2D-Lighting-Overview

    // Clear background
    BeginTextureMode(screenSpaceLightMap);
    ClearBackground(ambientColor);
    
    BeginBlendMode(BLEND_ADDITIVE);
    // Draw each light to buffer texture
    for(Light2D* light : lights)
    {
        Vector2 pos = light->GetGlobalPosition();
        Vector2 screenPos = GetWorldToScreen2D(light->GetGlobalPosition(), *currentCamera);
        //DrawCircle(screenPos.x, screenPos.y, light->distance * currentCamera->zoom, light->color);
        DrawCircleGradient(screenPos.x, screenPos.y, light->distance * currentCamera->zoom, light->color , ColorAlpha(light->color, 0.0));
       
    }
    EndBlendMode();
    EndTextureMode();
    
    //SetTextureFilter(screenSpaceLightMap.texture, TEXTURE_FILTER_BILINEAR);

    
    Rectangle lightSrc, lightDest;

    lightSrc = (Rectangle){0,0, (float)GetScreenWidth(), (float)-GetScreenHeight()};
    lightDest = (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()};

    BeginBlendMode(BLEND_MULTIPLIED);
    DrawTexturePro(screenSpaceLightMap.texture, lightSrc, lightDest, (Vector2){0,0}, 0, WHITE);
    EndBlendMode();
   
}

