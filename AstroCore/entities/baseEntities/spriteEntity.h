#include "entity.h"
#include "../../data/structs.h"
#include "../../engine/texturemanager.h"
#include "../../data/interfaces.h"

#ifndef SPRITE_ENTITY_H
#define SPRITE_ENTITY_H

namespace Astrolib
{
    class SpriteEntity : public Entity2D
    {

    public:
        SpriteEntity()
        {
            type = SPRITE;
            name = "SpriteEntity" + std::to_string(entityID);
        };
        SpriteEntity(Sprite *sprite);

        /// @brief Creates a new sprite entity
        /// @param spriteTexture The texture to use for the sprite
        /// @param spriteSize The original size of the sprite 
        /// @param center The center (rotation point) of the sprite
        SpriteEntity(Texture2D* spriteTexture, Vector2 spriteSize, Vector2 center);
        ~SpriteEntity();

        void Draw(float deltaTime, Camera2D *camera) override;
        Rectangle GetBoundRect() override;
        inline Sprite *GetSprite() { return sprite; };

        bool IsFlippedH() { return (spriteFlip.x == -1); };
        bool IsFlippedV() { return (spriteFlip.y == -1); };
        void SetFlipped(bool hFlipped, bool vFlipped)

        {
            spriteFlip.y = vFlipped ? -1 : 1;
            spriteFlip.x = hFlipped ? -1 : 1;
        };

        /// @brief Flip the sprite horizontally (local)
        void FlipH() { spriteFlip.x *= -1; };

        /// @brief Flip the sprite vertically (local)
        void FlipV() { spriteFlip.y *= -1; };

    protected:
        Vector2 spriteFlip = {1, 1};
        Sprite *sprite;
    };
}
#endif // SPRITE_ENTITY_H
