#include "stackedSpriteEntity.h"
#include "../../engine/scene.h"
using namespace Astrocore;

StackedSpriteEntity::StackedSpriteEntity(Texture2D *spriteSheet, int layersWide, int layerCount, 
                            Vector2 layerSize, Vector2 center, Vector2 spriteStartPos)
                            : SpriteEntity(spriteSheet, (Vector2){(float)spriteSheet->width, (float)spriteSheet->height}, center)
{
    // Note: The spritesheet width is passed into the constructor of the base sprite entity because the 'slicing' of each frame
    //          is performed in the Draw() of the stacked sprite sheet entity

    this->layersWide = layersWide;
    this->layerCount = layerCount;
    this->layerSize = layerSize;


}


void StackedSpriteEntity::SetLayerOffsetOverride(bool useOverride, float newLayerHeight)
{
    isOverridingOffset = useOverride;
    layerOffsetOverride = newLayerHeight;
}

void StackedSpriteEntity::Draw(float deltaTime, Camera2D *camera)
{
    // Update destination rect
    Rectangle srcRect, destRect;
    Vector2 globalPos = GetGlobalPosition();
    float srcPosX, srcPosY;

    CameraEntityBase* cameraEntity = currentScene->GetCurrentCamera();

    // Calculate and rotate normalized screen coords opposite of cameras rotation to maintain perspective
    Vector2 nSC = cameraEntity->GetNormalizedScreenCoords(globalPos);
    nSC = VectorHelper::RotateVectorAroundPoint(nSC, (Vector2){0,0}, -cameraEntity->GetGlobalRotation());

    //Debug::Log(std::to_string(nSC.y));
    // Calculate the layer offset we should use
    float offset = isOverridingOffset ? layerOffsetOverride : layerOffset;
    offset *= transform.scale.y * cameraEntity->zoom;
    
    bool shiftPerspective = isOverridingPerspective ? perspectiveShiftVal : globalUsePerspectiveShift;

    // Loop through each layer, offsetting and rendering above the bottom layer
    for (int i = 0; i < layerCount; i++)
    {
        srcPosX = i % layersWide * layerSize.x + spriteStartPos.x;
        srcPosY = (int)(i / layersWide) * layerSize.y + spriteStartPos.y;
        
        Vector2 offsetVec = {offset * i, offset * i};

        // Shift offset if we are using perspective shifting
        if(shiftPerspective)
        {
            offsetVec = VectorHelper::MultiplyVectors(offsetVec, {-nSC.x, -nSC.y});
        }

        srcRect = (Rectangle){srcPosX, srcPosY, layerSize.x, layerSize.y};
        destRect = (Rectangle){globalPos.x + offsetVec.x, globalPos.y + offsetVec.y , layerSize.x * transform.scale.x / pixelsPerUnit ,
                               layerSize.y * transform.scale.y / pixelsPerUnit};

        DrawTexturePro(*(sprite->spriteTexture),
                       srcRect,
                       destRect,
                       {(sprite->center.x * transform.scale.x) / pixelsPerUnit, (sprite->center.y * transform.scale.y) / pixelsPerUnit},
                       GetGlobalRotationDeg(),
                       WHITE);
    }
}
