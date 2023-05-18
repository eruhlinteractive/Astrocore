#include "../header/light2D.h"

using namespace Astrolib;

Light2D::Light2D()
{
    drawType = LIGHT;
}


Light2D::Light2D(float distance, float brightness, Color color) : Light2D()
{
    this->distance = distance;
    this->brightness = brightness;
    this->color = color;
}

void Light2D::Draw(float deltaTime)
{
  
}