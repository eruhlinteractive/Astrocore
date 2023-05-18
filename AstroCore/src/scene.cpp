#include "../header/scene.h"

using namespace Astrolib;

Scene::Scene()
{
    entities = std::map<std::string, Entity2D*>();
    LoadScene();
}

bool Scene::RegisterEntity(std::string name, Entity2D* entity)
{
    if(entity->GetChildCount() > 0)
    {
        std::vector<Entity2D*> children = entity->GetChildren();
        for(Entity2D* child : children)
        {
            RegisterEntity(std::to_string(child->GetId()), child);
        }
    }

    // Insert new reference
    if(entities.find(name) == entities.end())
    {
        entities.insert(std::pair{name, entity});
        return true;
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
    BeginMode2D(*currentCamera);
    std::vector<Entity2D*> pairs;
    // Create a list of all entities in the scene
    for (auto p : entities)
    {
        pairs.push_back((Entity2D*)p.second);
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
    

    // Draw every entity
    for(Entity2D* e : pairs)
    {
        // Don't draw if it's off screen

        Vector2 screenPos = GetWorldToScreen2D(e->GetPosition(), *currentCamera);
        // Screen border buffer to prevent pop-ins
        float rimBuffer = 100.0f * currentCamera->zoom;
        // AABB rect test to tell if we're on screen
        
        
        if(IsOnScreen(screenPos,rimBuffer))
        {
            e->Draw(deltaTime);
        }
        
    }

   EndMode2D();
   
}

