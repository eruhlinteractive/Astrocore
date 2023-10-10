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
        //~ShapeEntity();
        int AddCircle(Vector2 center, float radius, Color color);
        int AddRoundedRectOutline();
        int AddRoundedRect();
        int AddRectOutline();
        int AddRect();
        int AddLine(Vector2 startPos, Vector2 endPos, float width, Color color);

        SHAPE_TYPE TypeOfShape(int id);
        std::variant<LineData,CircleData> GetShape(int id);

        LineData* GetLine(int id);
        CircleData* GetCircle(int id);
        void SetShapeVisibility(int id, bool isVisible);
        bool IsShapeVisible(int id);
        
        
        /// @brief Delete a shape from the entity
        /// @param id The ID of the shape to delete
        void DeleteShape(int id);

        /// @brief Directly set the data for a shape
        /// @param id The id of the data to set
        /// @param data The data to set for the shape
        void SetShape(int id, void* data);

        virtual void Draw(float deltaTime, Camera2D* camera) override;

    private:
        std::map<int, std::variant<LineData, CircleData> > data;

        /// @brief A primary map to identify which shape type an ID is
        std::map<int, SHAPE_TYPE> shapeIdMap;
        std::map<int, bool> visibilityFlags;

        int shapeCount = 0;

    };
};
#endif // __VECTORSHAPEENTITY_H__