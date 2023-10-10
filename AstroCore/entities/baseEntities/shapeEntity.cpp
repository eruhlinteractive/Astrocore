#include "shapeEntity.h"

using namespace Astrocore;

ShapeEntity::ShapeEntity()
{
    data = std::map<int, std::variant<LineData, CircleData> > ();
    shapeIdMap = std::map<int, SHAPE_TYPE>();
    type = SPRITE;
}

int ShapeEntity::AddRoundedRectOutline()
{
    Debug::Assert(false, "Not yet Implemented");
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

    // Update maps
    shapeIdMap.insert({id, LINE});
    data.insert({id, LineData(startPos, endPos, width, color)});
    visibilityFlags.insert({id, true});

    return id;
}


int ShapeEntity::AddCircle(Vector2 center, float radius, Color color)
{
    int id = shapeCount;
    shapeCount++;

    shapeIdMap.insert({id, CIRCLE});
    data.insert({id, CircleData(center, radius, color)});
    visibilityFlags.insert({id, true});

    return id;
}


std::variant<LineData,CircleData> ShapeEntity::GetShape(int id)
{
    if(data.find(id) != data.end())
    {
        return data[id];
    }

    Debug::Assert(false, "Shape entity does not contain this id!");

    // Why does this work?
    return {};
}

void ShapeEntity::DeleteShape(int id)
{
    if(shapeIdMap.find(id) != shapeIdMap.end())
    {
        data.erase(id);
        shapeIdMap.erase(id);
        visibilityFlags.erase(id);
    }
}


LineData* ShapeEntity::GetLine(int id)
{
    if(data.find(id) == data.end())
    {
        return {};
    }

    // Make sure its a line
    if(shapeIdMap[id] != LINE)
    {
        Debug::Assert(shapeIdMap[id] == LINE, "Shape is not a line!");
        return {};
    }

    return &(std::get<LineData>(data[id]));
}


CircleData* ShapeEntity::GetCircle(int id)
{
    if(data.find(id) == data.end())
    {
        return {};
    }

    // Make sure its a line
    if(shapeIdMap[id] != CIRCLE)
    {
        Debug::Assert(shapeIdMap[id] == LINE, "Shape is not a line!");
        return {};
    }

    return &(std::get<CircleData>(data[id]));
}

void ShapeEntity::SetShapeVisibility(int id, bool isVisible)
{
    if(shapeIdMap.find(id) == shapeIdMap.end())
    {
        return;
    }
    if(visibilityFlags.find(id) != visibilityFlags.end())
    {
        visibilityFlags[id] = isVisible;
    }

}


void ShapeEntity::Draw(float deltaTime, Camera2D* camera)
{
    for(auto p : shapeIdMap)
    {
        // Skip invisible shapes
        if(!visibilityFlags[p.first])
        {
            continue;
        }

        // Render the rest
        SHAPE_TYPE type = p.second;
        float globalRot = GetGlobalRotation();
        Vector2 globalPos = GetGlobalPosition();

        switch (type)
        {
            // The brackets are used to keep initialized variables in the scope of the case handler
            case LINE:
                {
                    LineData l = std::get<LineData>(data[p.first]);

                    Vector2 startPos = VectorHelper::RotateVector(l.startPos,globalRot);
                    Vector2 endPos = VectorHelper::RotateVector(l.endPos, globalRot);
                    endPos = VectorHelper::AddVectors(globalPos, endPos);
                    startPos = VectorHelper::AddVectors(globalPos, startPos);

                    DrawLineEx(startPos, endPos, l.width, l.color);
                    break;
                }

            case CIRCLE:
                {
                    CircleData circ = std::get<CircleData>(data[p.first]);
                    Vector2 pos = VectorHelper::RotateVector(circ.center, globalRot);
                    pos = VectorHelper::AddVectors(globalPos, pos);
                    
                    DrawCircleV(pos, circ.radius, circ.color);
                     break;
                }
               
    
        };
    }
   
    
}
