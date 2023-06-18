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
            SetRenderResolution(GetScreenWidth(), GetScreenHeight());
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

            Vector2 newOffset = (Vector2){offset.x /samplingRatio, offset.y / samplingRatio };

            worldSpaceCamera->target.x = int(screenSpaceCamera->target.x);
            screenSpaceCamera->target.x -= worldSpaceCamera->target.x;
            screenSpaceCamera->target.x *= samplingRatio;

            worldSpaceCamera->target.y = int(screenSpaceCamera->target.y);
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

        void SetRenderResolution(float newRenderWidth, float newRenderHeight)
        {
            screenDimensions = (Vector2){(float)GetRenderWidth(), (float)GetRenderHeight()};
            samplingRatio = (float)screenDimensions.x / (float)newRenderWidth;
            virtualResolution = (Vector2){(float)newRenderWidth, (float)screenDimensions.y/ samplingRatio};

        
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
            SetTextureFilter(renderTexture.texture, TEXTURE_FILTER_POINT);

            // Calculate sub-pixel offsets
            //float offsetX = fmodf(screenSpaceCamera->target.x, 1.0f);
            //float offsetY = fmodf(screenSpaceCamera->target.y, 1.0f);;

            DrawTexturePro(renderTexture.texture, sourceRect, destRect, {0, 0}, 0.0f, WHITE);
            EndMode2D();
             
            std::string posText = std::to_string(screenSpaceCamera->target.x) + "," + std::to_string(screenSpaceCamera->target.y);
            DrawText(posText.c_str(), 10, 60, 20, RED);

            std::string wposText = std::to_string(worldSpaceCamera->target.x) + "," + std::to_string(worldSpaceCamera->target.y);
            DrawText(wposText.c_str(), 10, 85, 20, BLUE);
            //DrawRectangleLines(screenSpaceCamera->target.x, screenSpaceCamera->target.y,screenDimensions.x, screenDimensions.y, RED);
           
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