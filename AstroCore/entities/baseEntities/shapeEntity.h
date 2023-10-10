#ifndef __SHAPEENTITY_H__
#define __SHAPEENTITY_H__
#include "entity.h"
#include <variant>

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
        /// @param id The id of the data to set
        /// @param data The data to set for the shape
        void SetShape(int id, void* data);

        virtual void Draw(float deltaTime, Camera2D* camera) override;

    private:
        std::map<int, std::variant<LineData, Rectangle> > data;

        /// @brief A primary map to identify which shape type an ID is
        std::map<int, SHAPE_TYPE> shapeIdMap;
        int shapeCount = 0;

    };
};
#endif // __VECTORSHAPEENTITY_H__