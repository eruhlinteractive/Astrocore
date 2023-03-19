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

    Texture spriteTexture;
    int frameMax = 0;
    int currentFrame = 1;
    int animFPS = 10;
    int framesWide = 1 ;
    Vector2 origin;
    Vector2 frameSize;
};


#endif