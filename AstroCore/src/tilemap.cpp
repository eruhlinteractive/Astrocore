#include "../header/tilemap.h"



TileMap::TileMap()
{
    name = "TileMap" + std::to_string(entityID);
    drawType = TILEMAP;
    drawLayer = baseDrawLayer;
}

TileMap::TileMap(int tileSizeX, int tileSizeY, std::string name) : TileMap()
{
    this->tileSizeX = tileSizeX;
    this->tileSizeY = tileSizeY;
    this->name = name != "" ? name : "TileMap" + std::to_string(entityID);
}

void TileMap::Draw(float deltaTime)
{

}

/// @brief Load TMX data for a tilemap
/// @param filePath 
void TileMap::LoadDataTMX(std::string filePath)
{
    std::string fullPath = (std::string)GetWorkingDirectory() + "/" + filePath;
    //mapData.load(fullPath.c_str());
    // TODO: Load data from file manager 

    // XML parsing of main map file
    //std::string tileMapData = FileManager::instance().LoadFileAsText(filePath);
    //tinyxml2::XMLDocument doc;
    //doc.Parse(tileMapData.c_str());
    //std::string dirPath = (std::string)GetDirectoryPath(filePath.c_str());

    //tinyxml2::XMLElement* mapInfo = doc.FirstChildElement("map");
    //tinyxml2::XMLElement* tilesetInfo = mapInfo->FirstChildElement("tileset");

    //std::string tileMapSrcPath = dirPath + ((std::string)tilesetInfo->Attribute("source"));

    // Source : https://stackoverflow.com/questions/194465/how-to-parse-a-string-to-an-int-in-c
    //this->tileSizeX = std::stoi(mapInfo->Attribute("tilewidth"));
    //this->tileSizeY = std::stoi(mapInfo->Attribute("tileheight"));

}