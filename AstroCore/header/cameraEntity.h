#ifndef __CAMERAENTITY_H__
#define __CAMERAENTITY_H__

#include "cameraEntityBase.h"

namespace Astrolib
{
    class CameraEntity : public CameraEntityBase
    {
    public:
        CameraEntity()
        {
            camera = new Camera2D();
            type = CAMERA;
        }

        ~CameraEntity()
        {
            delete camera;
            camera = nullptr;
        }

        Camera2D* GetCamera() override
        {
            return camera;
        }

        void Update(float deltaTime) override
        {
            //camera->target = target;
            //camera->target = (Vector2){target.x, target.y};
            //camera->offset = offset;
            //camera->zoom = zoom;
        }

        void LateUpdate(float deltaTime) override
        {
            camera->target = target;
            camera->target = (Vector2){target.x, target.y};
            camera->offset = offset;
            camera->zoom = zoom;
        }

        // Only declared to satisfy the pure virtual function
        void Draw(float deltaTime, Camera2D *camera) override {}

    private:
        Camera2D* camera;
    };
}

#endif // __CAMERAENTITY_H__