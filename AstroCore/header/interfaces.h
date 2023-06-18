#ifndef __INTERFACES_H__
#define __INTERFACES_H__

#include <raylib.h>



namespace Astrolib
{
class IDrawable2D
{
    virtual void Draw(float deltaTime, Vector2 cameraPosition) = 0;
};

}

#endif // __INTERFACES_H__