#ifndef __TEXTUREMANAGER_H__
#define __TEXTUREMANAGER_H__

#include <string>
#include <map>
#include <raylib.h>

namespace Astrolib
{
    class TextureManager
    {
    public:
        static TextureManager &instance()
        {
            static TextureManager INSTANCE;
            return INSTANCE;
        }

        TextureManager();
        ~TextureManager();

        /// @brief Get a Texture2D, loading it if needed
        /// @param path The path to load the texture
        /// @return The loaded texture
        Texture2D GetTexture(std::string path);

        Texture2D GetTextureAbsolute(std::string absolutePath);
        std::string GetTexturePath(Texture2D texture);

        void UnloadAllTextures();
        /// @brief Remove a reference to a texture, unloading it if it is no longer referenced
        /// @param path The path to the texture
        void UnloadTexture(std::string path);
        void UnloadTexture(Texture2D texture);

    private:
        /// @brief The currently loaded textures
        std::map<std::string, Texture2D> *textures;

        /// @brief The amount of 'references' to each texture
        std::map<std::string, int> *textureReferences;
    };
}
#endif // __TEXTUREMANAGER_H__