#ifndef __VECTORSHAPEENTITY_H__
#define __VECTORSHAPEENTITY_H__
#include "entity.h"

namespace Astrocore
{
    class VectorShapeEntity : public Entity2D
    {
    public:
        VectorShapeEntity();
        int AddCircle();
        int AddRoundedRectOutline();
        int AddRoundedRect();
        int AddRectOutline();
        int AddRect();
        int AddLine();

    private:
        JSON data;
        int shapeCount = 0;

    }
};
#endif // __VECTORSHAPEENTITY_H__