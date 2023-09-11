#ifndef __STACKEDSPRITEENTITY_H__
#define __STACKEDSPRITEENTITY_H__

#include "spriteEntity.h"

namespace Astrocore
{
    class StackedSpriteEntity : public SpriteEntity
    {
    public:
        StackedSpriteEntity(Texture2D *spriteSheet, int layersWide, int layerCount,
                            Vector2 layerSize, Vector2 center, Vector2 spriteStartPos);
        void Draw(float deltaTime, Camera2D *camera) override;

        void SetLayerOffsetOverride(bool useOverride, float newLayerHeight);

        static inline void SetGlobalShiftPerspective(bool shouldShift) { globalUsePerspectiveShift = shouldShift;}

        inline void SetUsePerspectiveShiftOverride(bool override, bool usePerspectiveShift)
        {
            isOverridingPerspective = override;
            perspectiveShiftVal = usePerspectiveShift;
        }

        inline bool IsUsingPerspectiveShift() { return isOverridingPerspective ? perspectiveShiftVal : globalUsePerspectiveShift; }

        /// @brief Should the perspective shift based on direction to the camera?

    protected:
        ///@brief How many layers are in each row of the texture
        int layersWide = 0;

        ///@brief Total layer count
        int layerCount = 0;

        ///@brief The start position of the layer sprite (set for use with a sprite atlas)
        Vector2 spriteStartPos = (Vector2){0, 0};

        /// @brief The size of each layer
        Vector2 layerSize;

        /// @brief If the layer offset is being overriden, this value is used
        float layerOffsetOverride = 0.0f;

        /// @brief Should we override the global layer offset
        bool isOverridingOffset = false;

        /// @brief Are we overriding the global perspective value?
        bool isOverridingPerspective = false;

        /// @brief Used when we are overriding the global perspective shift
        bool perspectiveShiftVal = false;

    private:
        /// @brief Should perspective be shifted based on camera position
        static inline bool globalUsePerspectiveShift = false;

        // Static to help keep offset (i.e view angle) consistent across sprites
        static inline float layerOffset = -0.02f;
    };
}
#endif // __STACKEDSPRITEENTITY_H__