#include "../header/scene.h"

using namespace Astrolib;

Scene::Scene()
{
    entities = std::map<std::string, Entity2D*>();
}

bool Scene::RegisterEntity(std::string name, Entity2D* entity)
{
    // Insert new reference
    if(entities.find(name) == entities.end())
    {
        entities.insert(std::pair{name, entity});
        return true;
    }

    return false;
}


Entity2D* Scene::FindEntityByName(std::string name)
{
    if(entities.find(name) == entities.end())
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
    // Create a list of all entities in the scene
    for (auto p : entities)
    {
        pairs.push_back(p.second);
    }
    // Sort by draw layer
    
    std::sort(pairs.begin(), pairs.end(), SortByLayerIndex);

    
    // Draw every entity
    for(auto e : pairs)
    {
        e->Draw(deltaTime);
    }

   for(auto e : pairs)
   {
        e->Draw(deltaTime);
   }
   
}

