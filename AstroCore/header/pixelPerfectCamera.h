#ifndef __PIXELPERFECTCAMERA_H__
#define __PIXELPERFECTCAMERA_H__
#include "raylib.h"
#include "cameraEntityBase.h"

namespace Astrolib
{
    class PixelPerfectCamera2D : public CameraEntityBase
    {
    public:
        PixelPerfectCamera2D()
        {
            type = PIXELCAMERA;
            renderTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
            screenSpaceCamera = new Camera2D();
           
            worldSpaceCamera = new Camera2D();
            worldSpaceCamera->zoom = 1.0;
            screenSpaceCamera->zoom = 1.0;
            SetRenderWidth(480);
        }

        ~PixelPerfectCamera2D()
        {
            UnloadRenderTexture(renderTexture);
            
            delete worldSpaceCamera;
            worldSpaceCamera = nullptr;

            delete screenSpaceCamera;
            screenSpaceCamera = nullptr;
        }

        void Update(float deltaTime) override
        {
            screenSpaceCamera->target = target;

            // Update non-position parameters
            screenSpaceCamera->zoom = zoom;
            worldSpaceCamera->zoom = zoom;

            // Update non-position parameters
            //screenSpaceCamera->offset = {offset.x, offset.y};
            //worldSpaceCamera->offset = {-1.0 * offset.x, -1.0 * offset.y};
            

            Vector2 newOffset = (Vector2){offset.x /samplingRatio, offset.y / samplingRatio };

            worldSpaceCamera->target.x = (int)screenSpaceCamera->target.x;
            screenSpaceCamera->target.x -= worldSpaceCamera->target.x;
            
            screenSpaceCamera->target.x *= samplingRatio;

            worldSpaceCamera->target.y = (int)screenSpaceCamera->target.y;
            screenSpaceCamera->target.y -= worldSpaceCamera->target.y;
           
            screenSpaceCamera->target.y *= samplingRatio;

            worldSpaceCamera->target.y -= newOffset.y;
            worldSpaceCamera->target.x -= newOffset.x;
        }

        RenderTexture2D* GetRenderTexture(){ return &renderTexture; };

        Camera2D *GetCamera() override
        {
            return worldSpaceCamera;
        }

        Camera2D *GetScreenCamera(){
            return screenSpaceCamera;
        }

        void SetRenderWidth(float newRenderWidth)
        {
            screenDimensions = (Vector2){(float)GetRenderWidth(), (float)GetRenderHeight()};
            samplingRatio = screenDimensions.x / newRenderWidth;
            virtualResolution = (Vector2){(float)screenDimensions.x / samplingRatio, (float)screenDimensions.y / samplingRatio};

        
            // Re-initialize render texture
            UnloadRenderTexture(renderTexture);
            renderTexture = LoadRenderTexture(virtualResolution.x, virtualResolution.y);


            // The target's height is flipped (in the source Rectangle), due to OpenGL reasons
            sourceRect = {0.0f, 0.0f, (float)renderTexture.texture.width, -(float)renderTexture.texture.height};
            destRect = {-samplingRatio, -samplingRatio, screenDimensions.x + (samplingRatio * 2), screenDimensions.y + (samplingRatio * 2)};
        }

        // Draws the rendered texture to the screen
        void Draw(float deltaTime, Camera2D *camera) override
        {
            BeginMode2D(*screenSpaceCamera);
            SetTextureFilter(renderTexture.texture, RL_TEXTURE_FILTER_MIP_NEAREST);
            DrawTexturePro(renderTexture.texture, sourceRect, destRect, {0,0}, 0.0f, WHITE);
            //DrawRectangleLines(screenSpaceCamera->target.x, screenSpaceCamera->target.y,screenDimensions.x, screenDimensions.y, RED);
            EndMode2D();
        }
 
    private:

        RenderTexture2D renderTexture;
        Vector2 screenDimensions;
        float samplingRatio = 1.0;
        Vector2 virtualResolution;
        Camera2D *worldSpaceCamera;
        Camera2D *screenSpaceCamera;
        Rectangle sourceRect;
        Rectangle destRect;
    };
}

#endif // __PIXELPERFECTCAMERA_H__