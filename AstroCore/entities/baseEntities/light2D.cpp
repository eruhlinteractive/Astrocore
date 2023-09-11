#include "light2D.h"

using namespace Astrocore;

Light2D::Light2D()
{
    type = LIGHT;
    this->distance = 1.0;
    this->brightness = 1.0;
    this->color = WHITE;
}


Light2D::Light2D(float distance, float brightness, Color color) : Light2D()
{
    type = LIGHT;
    this->distance = distance;
    this->brightness = brightness;
    this->color = color;
}