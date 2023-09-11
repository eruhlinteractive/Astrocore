#ifndef __LIGHT_H__
#define __LIGHT_H__
#include "entity.h"

namespace Astrocore
{
    class Light2D : public Entity2D
    {
    public:
        Light2D();
        Light2D(float distance, float brightness, Color color);
        float distance;
        float brightness;
        Color color;

        void Draw(float deltaTime, Camera2D *camera) override {};
    };
}

#endif // __LIGHT_H__