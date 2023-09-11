#ifndef __TEXTUREMANAGER_H__
#define __TEXTUREMANAGER_H__

#include <string>
#include <map>
#include <raylib.h>
#include "debug.h"

namespace Astrocore
{
    class TextureManager
    {
    public:
        static TextureManager &instance()
        {
            if(!TextureManager::isInitialized)
            {
                TextureManager::INSTANCE = new TextureManager();
                TextureManager::isInitialized = true;
            }
          
            return *INSTANCE;
        }

        TextureManager();
        ~TextureManager();

        /// @brief Get a Texture2D, loading it if needed
        /// @param path The path to load the texture
        /// @return The loaded texture
        Texture2D* GetTexture(std::string path);

        Texture2D* GetTextureAbsolute(std::string absolutePath);
        std::string GetTexturePath(Texture2D* texture);

        void UnloadAllTextures();
        /// @brief Remove a reference to a texture, unloading it if it is no longer referenced
        /// @param path The path to the texture
        void UnloadTexture(std::string path);
        void UnloadTexture(Texture2D* texture);
    private:
        // Has the instance been initialized
        static inline bool isInitialized = false;

        inline static TextureManager* INSTANCE;

        /// @brief The currently loaded textures
        std::map<std::string, Texture2D> *textures;

        /// @brief The amount of 'references' to each texture
        std::map<std::string, int> *textureReferences;
    };
}
#endif // __TEXTUREMANAGER_H__