#ifndef __SHAPEENTITY_H__
#define __SHAPEENTITY_H__
#include "entity.h"

namespace Astrocore
{
    enum SHAPE_TYPE {CIRCLE, RR_OUTLINE, ROUNDED_RECT, RECT, LINE};
    class ShapeEntity : public Entity2D
    {
    
    public:
        ShapeEntity();
        int AddCircle();
        int AddRoundedRectOutline();
        int AddRoundedRect();
        int AddRectOutline();
        int AddRect();
        int AddLine(Vector2 startPos, Vector2 endPos, float width, Color color);

        void SetFloat(int shapeId, std::string key, float value);

        virtual void Draw(float deltaTime, Camera2D* camera) override;

    private:
        JSON Vector2ToJSON(Vector2 vector);
        std::vector<JSON> data;
        int shapeCount = 0;

    };
};
#endif // __VECTORSHAPEENTITY_H__