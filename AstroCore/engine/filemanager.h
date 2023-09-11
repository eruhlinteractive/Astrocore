#ifndef __FILEMANAGER_H__
#define __FILEMANAGER_H__

#include "../include/raylib.h"
#include <string>

namespace Astrocore
{
    class FileManager
    {
    public:
        // Singleton Creation
        static FileManager &instance()
        {
            static FileManager INSTANCE;
            return INSTANCE;
        }
        unsigned char *LoadFileBytes(std::string filePath);
        std::string LoadFileAsText(std::string filePath);
        bool FileExists(std::string path);
        bool SaveBytesToFile(std::string path, unsigned char *data);
    };
}

#endif // __FILEMANAGER_H__