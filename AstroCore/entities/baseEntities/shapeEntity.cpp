#include "shapeEntity.h"

using namespace Astrocore;

ShapeEntity::ShapeEntity()
{
    data = std::vector<JSON>();
    type = SPRITE;
}

int ShapeEntity::AddRoundedRectOutline()
{
    return 0;
}

/// @brief Add a line shape to this entity
/// @param startPos The starting position (local space)
/// @param endPos The end postion for the line (local space)
/// @param width The width of the line
/// @param color The color of the line
/// @return The id of the new line shape
int ShapeEntity::AddLine(Vector2 startPos, Vector2 endPos, float width, Color color)
{
    int id = shapeCount;
    shapeCount++;

    JSON shapeData = 
    {
        {"id", id},
        {"type", "LINE"},
        {"startX", startPos.x},
        {"startY", startPos.y},
        {"endX", endPos.x},
        {"endY", endPos.y},
        {"width", width},
        {"colorR", color.r},
        {"colorG", color.g},
        {"colorB", color.b},
        {"colorA", color.a},
    };
    
    data.push_back(shapeData);
    //Debug::Log("Added a new line");
    return id;
    
}


void ShapeEntity::Draw(float deltaTime, Camera2D* camera)
{
    for(JSON shapeData : data)
    {
        if(shapeData["type"] == "LINE")
        {
            //Debug::Log("There's a snake in my boot");
            Vector2 globalPos = GetGlobalPosition();
            Vector2 start = {shapeData["startX"], shapeData["startY"]};
            Vector2 end = {shapeData["endX"], shapeData["endY"]};

            float globalRot = GetGlobalRotation();
            start = VectorHelper::RotateVector(start, globalRot);
            end = VectorHelper::RotateVector(end, globalRot);

            end = VectorHelper::AddVectors(end, globalPos);            
            start = VectorHelper::AddVectors(start, globalPos);

            DrawLineEx(start, end, shapeData["width"], WHITE);
        }
    }

}
