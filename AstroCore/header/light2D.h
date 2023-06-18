#ifndef __LIGHT_H__
#define __LIGHT_H__
#include "entity.h"

namespace Astrolib
{
    class Light2D : public Entity2D
    {
    public:
        Light2D();
        Light2D(float distance, float brightness, Color color);
        float distance;
        float brightness;
        Color color;

        void Draw(float deltaTime, Vector2 cameraPosition) override {};
    };
}

#endif // __LIGHT_H__