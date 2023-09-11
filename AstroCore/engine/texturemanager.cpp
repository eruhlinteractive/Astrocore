#include "texturemanager.h"

using namespace Astrocore;


TextureManager::TextureManager()
{
    textures = new std::map<std::string, Texture2D>();
    textureReferences = new std::map<std::string, int>();
}


void TextureManager::UnloadAllTextures()
{
    if(textures == nullptr)
    {
        return;
    }

    Debug::Log("Unloading all textures");
    for(std::pair p : *textures)
    {   
        Debug::Log("Unloading texture with path: " + p.first);
        RLAPI::UnloadTexture((Texture2D)p.second);
    }    

    delete textures;
    textures = nullptr;

    delete textureReferences;
    textureReferences = nullptr;
}

TextureManager::~TextureManager()
{
    //UnloadAllTextures();
    delete textures;
    textures = nullptr;

    delete textureReferences;
    textureReferences = nullptr;
}

Texture2D* TextureManager::GetTextureAbsolute(std::string absolutePath)
{
    std::string fullPath = std::string(GetWorkingDirectory()) + "/" + absolutePath;
    return GetTexture(fullPath);
}

Texture2D* TextureManager::GetTexture(std::string path)
{
    // Texture not yet loaded, load and return
    if(textures->find(path) == textures->end())
    {
        //std::string fullPath = std::string(GetWorkingDirectory()) + "/" + path;
        textures->insert(std::pair{path, RLAPI::LoadTexture(path.c_str())});
        textureReferences->insert(std::pair{path, 1});
        return &(*textures)[path];
    }

    // Return loaded texture
    else
    {
        (*textureReferences)[path] += 1;
        return &(*textures)[path];
    }

}


std::string TextureManager::GetTexturePath(Texture2D* texture)
{
    if(textures == nullptr)
    {
        return "";
    }

    for(std::pair p : (*textures))
    {   
        if(p.second.id == texture->id)
        {
            return p.first;
        }
    }    
    return "";

}


void TextureManager::UnloadTexture(std::string path)
{

    if(textures->find(path) != textures->end())
    {
        (*textureReferences)[path] -= 1;
        
        // Texture is no longer referenced, unload it
        if((*textureReferences)[path] <= 0)
        {
            Debug::Log("Texture with path " + path + " has been deleted");
            textureReferences->erase(path);
            Texture2D texture = (*textures)[path];
            RLAPI::UnloadTexture(texture);
            textures->erase(path);
        }
    }

}


void TextureManager::UnloadTexture(Texture2D* texture)
{
    std::string path = this->GetTexturePath(texture);
    if(path != "")
    {
        
        UnloadTexture(path);
    }
   
}

