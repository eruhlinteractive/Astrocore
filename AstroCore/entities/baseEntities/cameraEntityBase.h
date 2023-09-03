#ifndef __CAMERAENTITYBASE_H__
#define __CAMERAENTITYBASE_H__
#include "raylib.h"
#include "entity.h"
#
// An interface to provide generalized properties for different types of cameras

enum VIEWPORT_SCALE_MODE {
    KEEP_ASPECT, // Keep aspect, letter boxing when needed
    FILL_ASPECT, // Fill the screen while keeping target render aspect ratio
    RENDER_FULL // Render the full screen at a 1:1 resolution (world is rendered at the screen resolution)
};

namespace Astrolib
{
    // TODO: Specalize a lot of this functionality out of the base class
    class CameraEntityBase : public Entity2D
    {
    public:
        ~CameraEntityBase()
        {
            UnloadRenderTexture(rendText);
        }
        // Make the calls to this a bit easier
        Vector2 target = {0, 0};
        Vector2 offset = {0, 0};
        float zoom = 1.0;

        virtual void SetRenderDimensions(float width, float height) = 0;

        // Pure virtual
        virtual Camera2D *GetCamera() = 0;

        virtual void BeginDrawing() = 0;
        virtual void EndDrawing() = 0;
        void Draw(float deltaTime, Camera2D *camera) = 0;
        
        virtual void OnNotify(const Signaler *signaler, std::string eventName) override
        {
            // This gets connected when the camera is registered with a scene
            if(eventName == "onWindowResized")
            {
                OnWindowResized();
            }
        }

        /// @brief Set the scale mode for the viewport
        /// @param scaleMode The scale mode to apply
        virtual void SetScaleMode(VIEWPORT_SCALE_MODE scaleMode)
        {
            maintainTargetAspectResolution = (scaleMode == KEEP_ASPECT);
        }

        virtual void OnWindowResized(){};

        Vector2 GetRenderCenter()
        {
            return (Vector2){renderResolution.x / 2.0f, renderResolution.y / 2.0f};
        }

        inline virtual Vector2 GetRenderResolution()
        {   
            return renderResolution;
        }

    protected:
        CameraEntityBase() : Entity2D()
        {
            type = CAMERA;
        };
        virtual void UpdateDestinationRectSize(){};
        bool maintainTargetAspectResolution = true;

        VIEWPORT_SCALE_MODE currentScaleMode = RENDER_FULL;

        /// @brief The resolution we are currently rendering the world at (dest texture)
        Vector2 renderResolution;
        RenderTexture2D rendText;
        Rectangle srcRect;
        Rectangle destRect;
    };

} // Astrolib

#endif // __CAMERAENTITYBASE_H__