#include "filemanager.h"

using namespace Astrocore;


unsigned char* FileManager::LoadFileBytes(std::string filePath)
{
    int fileLength = GetFileLength(filePath.c_str());

    // Potentially bad practice??
    return LoadFileData(filePath.c_str(), (unsigned int*)(&fileLength));

}

std::string FileManager::LoadFileAsText(std::string filePath)
{
    std::string fullPath = (std::string)GetWorkingDirectory() +"/" + filePath;
    char* data = RLAPI::LoadFileText(fullPath.c_str());
    std::string d = data;
    UnloadFileText(data);
    return d;
}