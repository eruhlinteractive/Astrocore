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
        ~ShapeEntity();
        int AddCircle();
        int AddRoundedRectOutline();
        int AddRoundedRect();
        int AddRectOutline();
        int AddRect();
        int AddLine(Vector2 startPos, Vector2 endPos, float width, Color color);

        SHAPE_TYPE TypeOfShape(int id);
        void* GetShape(int id);
        void DeleteShape(int id);

        /// @brief Directly set the data for a shape
        /// @param id 
        /// @param data 
        void SetShape(int id, void* data);

        virtual void Draw(float deltaTime, Camera2D* camera) override;

    private:
        JSON Vector2ToJSON(Vector2 vector);
        std::map<int, void*> data;

        /// @brief A primary map to identify which shape type an ID is
        std::map<int, SHAPE_TYPE> shapeIdMap;
        int shapeCount = 0;

    };
};
#endif // __VECTORSHAPEENTITY_H__