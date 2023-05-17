#include "../header/texturemanager.h"

using namespace Astrolib;


TextureManager::TextureManager()
{
    textures = std::map<std::string, Texture2D>();
    textureReferences = std::map<std::string, int>();
}


void TextureManager::UnloadAllTextures()
{
    for(std::pair p : textures)
    {   
        RLAPI::UnloadTexture((Texture2D)p.second);
    }    
}

TextureManager::~TextureManager()
{
    //delete textures;
    //textures = nullptr;

    //delete textureReferences;
    //textureReferences = nullptr;
}


Texture2D TextureManager::GetTexture(std::string path)
{
    // Texture not yet loaded, load and return
    if(textures.find(path) == textures.end())
    {
        Texture2D text = RLAPI::LoadTexture(path.c_str());
        textures.insert(std::pair{path, text});
        textureReferences.insert(std::pair{path, 1});
        return text;
    }

    // Return loaded texture
    else
    {
        textureReferences[path] += 1;
        return textures[path];
    }

}


std::string TextureManager::GetTexturePath(Texture2D texture)
{
    for(std::pair p : textures)
    {   
        if(p.second.id == texture.id)
        {
            return p.first;
        }
    }    
    return "";

}


void TextureManager::UnloadTexture(std::string path)
{

    if(textures.find(path) != textures.end())
    {
        
        textureReferences[path] -= 1;
        // Texture is no longer referenced, unload it
        if(textureReferences[path] <= 0)
        {
            textureReferences.erase(path);
            Texture2D texture = textures[path];
            RLAPI::UnloadTexture(texture);
            textures.erase(path);
        }
    }

}


void TextureManager::UnloadTexture(Texture2D texture)
{
    std::string path = GetTexturePath(texture);
    if(path != "")
    {
         UnloadTexture(path);
    }
   
}

