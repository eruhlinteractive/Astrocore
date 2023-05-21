#include "../header/filemanager.h"

using namespace Astrolib;


unsigned char* FileManager::LoadFileBytes(std::string filePath)
{
    int fileLength = GetFileLength(filePath.c_str());

    // Potentially bad practice??
    return LoadFileData(filePath.c_str(), (unsigned int*)(&fileLength));

}

std::string FileManager::LoadFileAsText(std::string filePath)
{
    char* data = RLAPI::LoadFileText(filePath.c_str());
    std::string d = data;
    UnloadFileText(data);
    return d;
}