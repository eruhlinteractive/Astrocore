
#include <math.h>

#ifndef STRUCTS_H
#define STRUCTS_H

/// @brief A struct representing a single input event (i.e an 'E' key press or 'A' button press on a controller)
struct InputEvent
{
    // Constructor
    InputEvent(int inputIndex, int actionControllerId =-1) 
    {
        controllerId = actionControllerId;
        positiveInput = inputIndex;
    };
    
    /// Is this a controller action?
    int controllerId = -1;

    // What index is the positive(on) input?
    int positiveInput;
};

struct Sprite
{
    Texture spriteTexture;
    Vector2 origin;
    Sprite(Texture2D texture, Vector2 origin)
    {
        this->spriteTexture = texture;
        this->origin = origin;
    }
};

struct SpriteAnimation
{
    SpriteAnimation()
    {
        frameMax = 0;
        currentFrame = 1;
        animFPS = 10;
        framesWide = 1;
        origin = {0,0};
        frameSize = {0,0};
    };
    
    SpriteAnimation(Texture2D sprite, int frameCount, int animFPS, int framesWide, Vector2 origin, Vector2 frameSize)
    {
        spriteTexture = sprite;
        frameMax = frameCount;
        currentFrame = 0;
        this->animFPS = animFPS;
        this->framesWide = framesWide;
        this->origin = origin;
        this->frameSize = frameSize;
    };

    Texture2D spriteTexture;
    int frameMax = 0;
    int currentFrame = 1;
    int animFPS = 10;
    int framesWide = 1;
    Vector2 origin;
    Vector2 frameSize;
};

struct Transform2D
{
    Vector2 position;
    float rotation;
    Vector2 scale;

    Transform2D()
    {
        position = (Vector2){0,0};
        rotation = 0;
        scale = (Vector2){1,1};
    }

    Transform2D(Vector2 position, float rotation, Vector2 scale)
    {
        this->position = position;
        this->rotation = rotation;
        this->scale = scale;
    }

    void MoveGlobal(Vector2 movement)
    {
        position = (Vector2){position.x + movement.x, position.y + movement.y};
    }

    void Rotate(float deltaRotationRadians)
    {
        rotation += deltaRotationRadians;
    }

    void RotateDegrees(float deltaRotationDeg)
    {
        Rotate((deltaRotationDeg * ((float)PI / 180.0f)));
    }

    void Scale(Vector2 scaleDelta)
    {
        scale = (Vector2){scale.x * scaleDelta.x, scale.y * scaleDelta.y};
    }

    /// @brief Move relative to the current rotation
    /// @param movement The delta movement vector
    void MoveLocal(Vector2 movement)
    {
        Vector2 movLocal = movement;
        movLocal.x = movement.x * cos(rotation) - movement.y * sin(rotation);
        movLocal.y = movement.x * sin(rotation) + movement.y * cos(rotation);

        position = (Vector2){movLocal.x + position.x, movLocal.y + position.y};
    }

    void RotateAround(Vector2 point, float deltaRotationRadians)
    {
        Vector2 translation = {position.x - point.x, position.y - point.y};

        float xNew = translation.x * cos(deltaRotationRadians) - translation.y * sin(deltaRotationRadians);
        float yNew = translation.x * sin(deltaRotationRadians) + translation.y * cos(deltaRotationRadians);

        translation = {xNew + point.x, yNew + point.y};

        Vector2 movement = {translation.x - position.x, translation.y - position.y};

        rotation += deltaRotationRadians;

        MoveGlobal(movement);
    }

    void SetRotationDeg(float newRotationDeg)
    {
        rotation = (newRotationDeg * M_PI / 180);
        // Keep within range 0-> 2PI
        rotation = std::fmod(rotation, (2.0f * M_PI));
    }
};

#endif