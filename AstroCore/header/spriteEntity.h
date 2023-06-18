#include "entity.h"
#include "structs.h"
#include "texturemanager.h"
#include "interfaces.h"

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
        SpriteEntity(Texture2D spriteTexture, Vector2 frameSize, Vector2 origin);
        ~SpriteEntity();

        void Draw(float deltaTime, Camera2D* camera) override;
        Rectangle GetSpriteRect() override;
        
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
