#ifndef __PIXELPERFECTCAMERA_H__
#define __PIXELPERFECTCAMERA_H__
#include "raylib.h"
#include "cameraEntityBase.h"

namespace Astrolib
{
    class PixelPerfectCamera2D : public CameraEntityBase
    {
    public:
        Vector2 targetPos = {0,0};
        PixelPerfectCamera2D() : CameraEntityBase()
        {
            type = PIXELCAMERA;
            renderTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
            screenSpaceCamera = new Camera2D();
           
            worldSpaceCamera = new Camera2D();
            worldSpaceCamera->zoom = 1.0;
            screenSpaceCamera->zoom = 1.0;
            SetRenderDimensions(GetScreenWidth(), GetScreenHeight());
        }

        ~PixelPerfectCamera2D()
        {
            UnloadRenderTexture(renderTexture);
            
            delete worldSpaceCamera;
            worldSpaceCamera = nullptr;

            delete screenSpaceCamera;
            screenSpaceCamera = nullptr;
        }

        void LateUpdate(float deltaTime) override
        {
            screenSpaceCamera->target = target;
            targetPos = target;
            Vector2 newOffset = (Vector2){offset.x / samplingRatio, offset.y / samplingRatio };
            worldSpaceCamera->offset = newOffset;

        
            worldSpaceCamera->target.x = (int)screenSpaceCamera->target.x;
            screenSpaceCamera->target.x -= worldSpaceCamera->target.x;
            //screenSpaceCamera->target.x *= samplingRatio;

            worldSpaceCamera->target.y = int(screenSpaceCamera->target.y);
            screenSpaceCamera->target.y -= worldSpaceCamera->target.y;
            //screenSpaceCamera->target.y *= samplingRatio;
        }

        RenderTexture2D* GetRenderTexture(){ return &renderTexture; };

        Camera2D *GetCamera() override
        {
            return worldSpaceCamera;
        }

        Camera2D *GetScreenCamera(){
            return screenSpaceCamera;
        }

        void BeginDrawing() override { /* TODO: Implement this*/};
        void EndDrawing() override { /* TODO: Implement this*/};

        void SetRenderDimensions(float width, float height, bool maintainAspect = true) override
        {
            screenDimensions = (Vector2){(float)GetRenderWidth(), (float)GetRenderHeight()};
            samplingRatio = (float)screenDimensions.x / (float)width;
            virtualResolution = (Vector2){(float)width, height};

        
            // Re-initialize render texture
            UnloadRenderTexture(renderTexture);
            renderTexture = LoadRenderTexture(virtualResolution.x, virtualResolution.y);

            // Update base-class render resolution
            renderResolution = {virtualResolution.x, virtualResolution.y};
            //SetTextureFilter(renderTexture.texture, TEXTURE_FILTER_POINT);

            // Update PPU used for movement/rendering
            //Entity2D::pixelsPerUnit = 1.0f / samplingRatio;


            // The target's height is flipped (in the source Rectangle), due to OpenGL reasons
            sourceRect = {0.0f, 0.0f, (float)renderTexture.texture.width, -(float)renderTexture.texture.height};
            destRect = {-samplingRatio, -samplingRatio, screenDimensions.x + samplingRatio*2.0f, screenDimensions.y + samplingRatio*2.0f};
        }

        // Draws the rendered texture to the screen
        void Draw(float deltaTime, Camera2D *camera) override
        {
            BeginMode2D(*screenSpaceCamera); 
            
            // Calculate sub-pixel offsets
            //float offsetX = fmodf(screenSpaceCamera->target.x, 1.0f);
            //float offsetY = fmodf(screenSpaceCamera->target.y, 1.0f);;

            DrawTexturePro(renderTexture.texture, sourceRect, destRect, {0, 0}, 0.0f, WHITE);
            EndMode2D();
             

            /*
            std::string posText = "Target:" + std::to_string(targetPos.x) + "," + std::to_string(targetPos.y);
            DrawText(posText.c_str(), 10, 60, 20, RED);

            std::string wposText = std::to_string(screenSpaceCamera->target.x) + "," + std::to_string(screenSpaceCamera->target.y);
            DrawText(wposText.c_str(), 10, 85, 20, BLUE);

            std::string vrestText = "VRes: " + std::to_string(virtualResolution.x) + "," + std::to_string(virtualResolution.y);
            DrawText(vrestText.c_str(), 10, 105, 20, BLUE);
            */

           
           //DrawRectangleLines(200, 200, 1 * samplingRatio, 1 * samplingRatio, RED);
           //DrawRectangleLines(210, 200, 1, 1 , RED);

           //DrawRectangleLines(screenSpaceCamera->target.x, screenSpaceCamera->target.y ,destRect.width, destRect.height, RED);
           
        }
        
    private:

        RenderTexture2D renderTexture;
        Vector2 screenDimensions;
        float samplingRatio;
        Vector2 virtualResolution;
        Camera2D *worldSpaceCamera;
        Camera2D *screenSpaceCamera;
        Rectangle sourceRect;
        Rectangle destRect;
    };
}

#endif // __PIXELPERFECTCAMERA_H__