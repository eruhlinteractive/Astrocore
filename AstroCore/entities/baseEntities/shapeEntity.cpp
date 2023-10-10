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
}



void ShapeEntity::Draw(float deltaTime, Camera2D* camera)
{
    for(std::pair<int, SHAPE_TYPE> p : shapeIdMap)
    {
        SHAPE_TYPE type = p.second;

    }
   
    
}
