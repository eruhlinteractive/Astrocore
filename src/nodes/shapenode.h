#ifndef SHAPENODE
#define SHAPENODE

#include <vector>

#ifndef RAYLIB_H
#include <raylib.h>
#include <raymath.h>
#endif // !RAYLIB_H

#include <cmath>
#include "node.h"

namespace Astrocore
{
    struct Shape;
    // A node that draws geometric shapes
    class ShapeNode : public Node
    {
    private:
        std::vector<Shape> shapesToDraw;

    public:
        ShapeNode();
        ShapeNode(Shape initialShape);
        void AddShape(Shape newShape);
        void Draw();

    };

struct Shape
{
    std::vector<Vector2> points;
    bool isFilled = false;
    Color color = RED;
    Vector2 center = {0,0};
    float rotationOffset = 0;
    float lineWidth = 1.0f;
    float isClosed = true;

    Shape()
    {
        points = std::vector<Vector2>();
    };

    Shape(Vector2 center, float rotOffset = 0)
    {
        this->center = center;
        rotationOffset = rotOffset;
    }

    void AsRect(float height, float width)
    {
        points.clear();
        points.push_back({height, width});
        points.push_back({-height, width});
        points.push_back({-height, -width});
        points.push_back({height, -width});
    }

    void AsCircle(float radius, int resolution)
    {
        float pointOffset = (2.0 * PI) / resolution;
        float currAngle = 0;
        points.clear();
        for(int i= 0; i < resolution; i++)
        {
            currAngle = i * pointOffset;
            points.push_back({cos(currAngle) * radius, sin(currAngle)* radius});
        }
    }
};
}


#endif // !SHAPENODE