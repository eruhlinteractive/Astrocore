#ifndef __CAMERAENTITYBASE_H__
#define __CAMERAENTITYBASE_H__
#include "raylib.h"
#include "entity.h"

// An interface to provide generalized properties for different types of cameras

namespace Astrolib
{
    // TODO: Specalize a lot of this functionality out of the base class
    class CameraEntityBase : public Entity2D
    {
    public:
        CameraEntityBase()
        {
            type = CAMERA;
            SetRenderDimensions(GetRenderWidth(), GetRenderHeight());
        };

        // Make the calls to this a bit easier
        Vector2 target = {0, 0};
        Vector2 offset = {0, 0};
        float zoom = 1.0;

        void SetRenderDimensions(float width, float height, bool maintainAspect = true)
        {
            maintainTargetAspectResolution = maintainAspect;
            renderResolution = (Vector2){width, height};
            UnloadRenderTexture(rendText);
            rendText = LoadRenderTexture(width, height);
            srcRect = (Rectangle){0, 0, width, -height};
        }

        void UpdateDestinationRectSize()
        {
            if (maintainTargetAspectResolution)
            {
                float scale = min(GetRenderWidth() / renderResolution.x, GetRenderHeight() / renderResolution.y);
                destRect = {
                    (GetRenderWidth() - ((float)renderResolution.x * scale)) * 0.5f,
                    (GetRenderHeight() - ((float)renderResolution.y * scale)) * 0.5f,
                    (float)renderResolution.x * scale,
                    (float)renderResolution.y * scale};
            }
            else
            {
                destRect = (Rectangle){0, 0, GetRenderWidth(), GetRenderHeight()};
            }
        }

        // Pure virtual
        virtual Camera2D *GetCamera() = 0;

        void BeginDrawing()
        {
            BeginTextureMode(rendText);
            BeginMode2D(*GetCamera());
        }

        void EndDrawing()
        {

            EndMode2D();
            EndTextureMode();
        }

        Vector2 GetRenderCenter()
        {
            return (Vector2){renderResolution.x / 2.0f, renderResolution.y / 2.0f};
        }

        void Draw(float deltaTime, Camera2D *camera) override
        {
            ClearBackground(BLACK);
            DrawTexturePro(rendText.texture, srcRect, destRect, {0, 0}, 0, WHITE);
        };

    protected:
        bool maintainTargetAspectResolution = true;
        Vector2 renderResolution;
        RenderTexture2D rendText;
        Rectangle srcRect;
        Rectangle destRect;
    };

} // Astrolib

#endif // __CAMERAENTITYBASE_H__