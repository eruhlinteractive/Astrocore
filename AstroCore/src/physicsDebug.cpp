#include "../header/physicsDebug.h"

using namespace Astrolib;
PhysicsDebug::PhysicsDebug()
{
    SetFlags(e_shapeBit);
    AppendFlags(e_centerOfMassBit);
}

void PhysicsDebug::DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color)
{
    Vector2 *points = new Vector2[vertexCount]();
    points += vertexCount + 1;

    if (vertexCount == 0)
    {
        return;
    }
    b2Vec2 firstVert = *vertices;
    b2Vec2 finalVert;
    for (int i = 0; i < vertexCount; i++)
    {

        const b2Vec2 vert = *vertices;
        // Reverse winding order of the points array (b2d is CCW, openGL is Cw)
        *(--points) = (Vector2){vert.x, vert.y};
        vertices++;
    }
    Vector2 point = *points;
    finalVert = b2Vec2(point.x, point.y);

    DrawLineStrip(points, vertexCount, (Color){color.r * 255.0f, color.g* 255.0f, color.b* 255.0f, color.a* 255.0f});
    Vector2 *finalPoints = new Vector2[vertexCount]();

    // Close the polygon
    *(finalPoints) = {finalVert.x, finalVert.y};
    *(finalPoints + 1) = {firstVert.x, firstVert.y};
    DrawLineStrip(finalPoints, 2, (Color){color.r* 255.0f, color.g* 255.0f, color.b* 255.0f, color.a* 255.0f});
}

void PhysicsDebug::DrawTransform(const b2Transform &xf)
{
    DrawCircleV({xf.p.x, xf.p.y}, 0.5f, PURPLE);

    b2Vec2 vec = {cos(xf.q.GetAngle()) * 5, sin(xf.q.GetAngle()) * 5};
    DrawLineV({xf.p.x, xf.p.y}, {xf.p.x + vec.x, xf.p.y + vec.y}, PURPLE);
}