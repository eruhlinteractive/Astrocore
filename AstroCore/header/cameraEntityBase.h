#ifndef __CAMERAENTITYBASE_H__
#define __CAMERAENTITYBASE_H__
#include "raylib.h"
#include "entity.h"

// An interface to provide generalized properties for different types of cameras

namespace Astrolib
{
    class CameraEntityBase : public Entity2D
    {
    public:
        CameraEntityBase(){type = CAMERA;};

        // Make the calls to this a bit easier
        Vector2 target = {0,0};
        Vector2 offset = {0,0};
        float zoom = 1.0;

        // Pure virtual 
        virtual Camera2D* GetCamera() = 0;

        void Draw(float deltaTime, Camera2D* camera) override {};
    };

} // Astrolib

#endif // __CAMERAENTITYBASE_H__