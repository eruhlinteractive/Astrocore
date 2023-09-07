#include "physicsDebug.h"
#include <vector>

using namespace Astrolib;
PhysicsDebug::PhysicsDebug()
{
    SetFlags(e_shapeBit);
    AppendFlags(e_centerOfMassBit);
}

void PhysicsDebug::DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color)
{
    if (vertexCount == 0)
    {
        return;
    }
    
    std::vector<Vector2> points = std::vector<Vector2>();

    b2Vec2 firstVert = *vertices;
    b2Vec2 finalVert;
    for (int i = 0; i < vertexCount; i++)
    {

        const b2Vec2 vert = *vertices;
        points.push_back((Vector2){vert.x, vert.y});
        vertices++;
    }

    DrawLineStrip(points.data(), vertexCount, (Color){(unsigned char)(color.r * 255.0f), (unsigned char)(color.g * 255.0f), (unsigned char)(color.b * 255.0f), (unsigned char)(color.a * 255.0f)});

    // Close the loop
    std::vector<Vector2> finalPoints = std::vector<Vector2>();
    finalPoints.push_back(points[points.size() - 1]);
    finalPoints.push_back(points[0]);

    DrawLineStrip(finalPoints.data(), 2, (Color){(unsigned char)(color.r * 255.0f), (unsigned char)(color.g * 255.0f), (unsigned char)(color.b * 255.0f), (unsigned char)(color.a * 255.0f)});
}

void PhysicsDebug::DrawTransform(const b2Transform &xf)
{
    //DrawCircleV({xf.p.x, xf.p.y}, 0.5f, PURPLE);
    float length = 10.0f;

    // Calculate vector along axis (1, O)
    b2Vec2 xVec = {cos(xf.q.GetAngle()), sin(xf.q.GetAngle())};
    // Normalize xAxis vector
    //float mag = sqrt(xVec.x * xVec.x + xVec.y * xVec.y);
    //xVec = {xVec.x / mag, xVec.y / mag};

    DrawLineV({xf.p.x, xf.p.y}, {xf.p.x + xVec.x * length , xf.p.y + xVec.y * length}, RED);

    // Calculate and draw Y axis vector
    float rot = xf.q.GetAngle() + -(PI/2.0f);

    b2Vec2 yVec = {cos(rot), sin(rot)};
    DrawLineV({xf.p.x, xf.p.y}, {xf.p.x + yVec.x * length, xf.p.y + yVec.y * length}, GREEN);


}