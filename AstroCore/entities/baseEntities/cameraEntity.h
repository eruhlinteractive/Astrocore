#ifndef __CAMERAENTITY_H__
#define __CAMERAENTITY_H__

#include "cameraEntityBase.h"

namespace Astrolib
{
    class CameraEntity : public CameraEntityBase
    {
    public:
        void Init() override
        {
            SetRenderDimensions(GetRenderWidth(), GetRenderHeight());
            UpdateDestinationRectSize();
        }

        virtual void OnWindowResized() override
        {
            if(currentScaleMode == RENDER_FULL)
            {
                SetRenderDimensions(GetRenderWidth(), GetRenderHeight());
                UpdateDestinationRectSize();
            }
            return;
            UpdateDestinationRectSize();
        }

        void BeginDrawing() override
        {
            BeginTextureMode(rendText);
            BeginMode2D(*GetCamera());
        }

        void EndDrawing() override
        {

            EndMode2D();
            EndTextureMode();
        }

        void Draw(float deltaTime, Camera2D *camera) override
        {
            //ClearBackground(BLACK);
            DrawTexturePro(rendText.texture, srcRect, destRect, {0, 0}, 0, WHITE);
        };


        void SetRenderDimensions(float width, float height) override
        {
            renderResolution = (Vector2){width, height};
            UnloadRenderTexture(rendText);
            rendText = LoadRenderTexture(width, height);
            //SetTextureFilter(rendText.texture, TEXTURE_FILTER_ANISOTROPIC_16X);
            srcRect = (Rectangle){0, 0, width, -height};
        }

        void UpdateDestinationRectSize() override
        {
            if (currentScaleMode == KEEP_ASPECT)
            {
                float scale = min(GetRenderWidth() / renderResolution.x, GetRenderHeight() / renderResolution.y);
                destRect = {
                    (GetRenderWidth() - ((float)renderResolution.x * scale)) * 0.5f,
                    (GetRenderHeight() - ((float)renderResolution.y * scale)) * 0.5f,
                    (float)renderResolution.x * scale,
                    (float)renderResolution.y * scale};
            }
            else if( currentScaleMode == FILL_ASPECT)
            {
                float scale = min(GetRenderWidth() / renderResolution.x, GetRenderHeight() / renderResolution.y);
            }
            else
            {
                destRect = (Rectangle){0, 0, (float)GetRenderWidth(), (float)GetRenderHeight()};
            }
        }


        virtual void SetScaleMode(VIEWPORT_SCALE_MODE scaleMode) override
        {
            CameraEntityBase::SetScaleMode(scaleMode);
            UpdateDestinationRectSize();
        }

        CameraEntity() : CameraEntityBase()
        {
            camera = new Camera2D();
            type = CAMERA;
            Init();
        }

        ~CameraEntity()
        {
            delete camera;
            camera = nullptr;
        }

        Camera2D *GetCamera() override
        {
            return camera;
        }

        void LateUpdate(float deltaTime) override
        {
            camera->target = target;
            camera->target = (Vector2){target.x, target.y};
            transform.position = {target.x, target.y};

            camera->offset = {offset.x + GetRenderCenter().x, offset.y + GetRenderCenter().y};
            camera->zoom = zoom;
        }

        // Only declared to satisfy the pure virtual function
        // void Draw(float deltaTime, Camera2D *camera) override {}

    private:

        // World space camera
        Camera2D *camera;
    };
}

#endif // __CAMERAENTITY_H__