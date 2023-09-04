
#include <math.h>
#ifndef STRUCTS_H
#define STRUCTS_H

namespace Astrolib
{
    /// @brief A struct representing a single input event (i.e an 'E' key press or 'A' button press on a controller)
    typedef struct InputEvent
    {
        // Constructor
        InputEvent(int inputIndex, int actionControllerId = -1)
        {
            controllerId = actionControllerId;
            positiveInput = inputIndex;
        };

        /// Is this a controller action?
        int controllerId = -1;

        // What index is the positive(on) input?
        int positiveInput;
    } InputEvent;

    typedef struct Sprite
    {
        // Texture that contains the sprite
        Texture *spriteTexture;

        // Middle point of the sprite
        Vector2 center;

        // Size of the sprite on the texture
        Vector2 spriteSize;

        // Start point of the sprite on the texture
        Vector2 startPoint;

        Sprite(Texture2D *texture, Vector2 center)
        {
            this->spriteTexture = texture;
            this->center = center;
            this->startPoint = (Vector2){0, 0};
            this->spriteSize = (Vector2){(float)texture->width, (float)texture->height};
        }

        Sprite(Texture2D *texture, Vector2 center, Vector2 startPoint, Vector2 spriteSize) : Sprite(texture, center)
        {
            this->startPoint = startPoint;
            this->spriteSize = spriteSize;
        }

    } Sprite;

    typedef struct SpriteAnimation
    {
        SpriteAnimation()
        {
            startPos = (Vector2){0, 0};
            frameMax = 0;
            currentFrame = 1;
            animFPS = 10;
            framesWide = 1;
            origin = {0, 0};
            frameSize = {0, 0};
        };
        SpriteAnimation(Texture2D *sprite, int frameCount, int animFPS, int framesWide, Vector2 origin, Vector2 frameSize)
        {
            spriteTexture = sprite;
            this->startPos = (Vector2){0, 0};
            frameMax = frameCount;
            currentFrame = 0;
            this->animFPS = animFPS;
            this->framesWide = framesWide;
            this->origin = origin;
            this->frameSize = frameSize;
        };

        SpriteAnimation(Texture2D *sprite, int frameCount, int animFPS, int framesWide, Vector2 startPos, Vector2 origin, Vector2 frameSize) : SpriteAnimation(sprite, frameCount, animFPS, framesWide, origin, frameSize)
        {
            this->startPos = startPos;
        };

        /// @brief The texture to draw from
        Texture2D *spriteTexture;

        /// @brief How many frames are in the animation
        int frameMax = 0;

        /// @brief Currently rendered frame
        int currentFrame = 1;

        /// @brief Framerate of the animation
        int animFPS = 10;

        /// @brief How many frames of this animation exist horizontally
        int framesWide = 1;

        /// @brief Center of the frame dimensions (i.e for a 16x16 sprite, origin would be 8,8)
        Vector2 origin;

        /// @brief The size of each frame
        Vector2 frameSize;

        /// @brief The start position (top left) of the first sprite
        Vector2 startPos;
    } SpriteAnimation;

    typedef struct Transform2D
    {
        // Local position
        Vector2 position;
        // Rotation in radians
        float rotation;

        // Local scale
        Vector2 scale;

        Transform2D()
        {
            this->position = (Vector2){0, 0};
            this->rotation = 0;
            this->scale = (Vector2){1, 1};
        }

        Transform2D(Vector2 position, float rotation, Vector2 scale)
        {
            this->position = position;
            this->rotation = rotation;
            this->scale = scale;
        }

        void MoveGlobal(Vector2 movement)
        {
            this->position = (Vector2){position.x + movement.x, position.y + movement.y};
        }

        void Rotate(float deltaRotationRadians)
        {
            this->rotation += deltaRotationRadians;
        }

        void RotateDegrees(float deltaRotationDeg)
        {
            Rotate((deltaRotationDeg * ((float)PI / 180.0f)));
        }

        void Scale(Vector2 scaleDelta)
        {
            this->scale = (Vector2){scale.x * scaleDelta.x, scale.y * scaleDelta.y};
        }

        /// @brief Move relative to the current rotation
        /// @param movement The delta movement vector
        void MoveLocal(Vector2 movement)
        {
            Vector2 movLocal = movement;
            movLocal.x = movement.x * cos(rotation) - movement.y * sin(rotation);
            movLocal.y = movement.x * sin(rotation) + movement.y * cos(rotation);

            this->position = (Vector2){movLocal.x + position.x, movLocal.y + position.y};
        }

        void RotateAround(Vector2 point, float deltaRotationRadians)
        {
            Vector2 translation = {position.x - point.x, position.y - point.y};

            float xNew = translation.x * cos(deltaRotationRadians) - translation.y * sin(deltaRotationRadians);
            float yNew = translation.x * sin(deltaRotationRadians) + translation.y * cos(deltaRotationRadians);

            translation = {xNew + point.x, yNew + point.y};

            Vector2 movement = {translation.x - position.x, translation.y - position.y};

            this->rotation += deltaRotationRadians;

            MoveGlobal(movement);
        }

        void SetRotationDeg(float newRotationDeg)
        {
            this->rotation = (newRotationDeg * M_PI / 180);
            // Keep within range 0-> 2PI
            this->rotation = std::fmod(rotation, (2.0f * M_PI));
        }
    } Transform2D;

    // Custom data to pass into physics objects
    typedef struct PhysicsEntityData
    {
        PhysicsEntityData(int entityID, std::string entityName)
        {
            this->entityID = entityID;
            this->entityName = entityName;
        }

        int entityID;
        std::string entityName;

        std::string GetEntityName() { return entityName; };
        int GetEntityId() { return entityID; };

    } PhysicsEntityData;

    /// @brief A set of helper functions for vector arithmetic
    typedef struct VectorHelper
    {
        ///@brief Rotates a vector by a given rotation (degrees)
        /// @param x The x component of the vector to rotate
        /// @param y The y component of the vector to rotate
        /// @param rotationDegrees The delta rotation in degrees
        /// @returns The rotated vector
        static Vector2 RotateVectorDegrees(Vector2 vector, float rotationDegrees)
        {
            float rotation = rotationDegrees * (PI / 180.0f);

            return RotateVector(vector, rotation);
            return (Vector2){0, 0};
        }

        /// @brief Rotates a vector by a given rotation (radians)
        /// @param x The x component of the vector to rotate
        /// @param y The y component of the vector to rotate
        /// @param rotationDegrees The delta rotation in radians
        /// @returns The rotated vector
        static Vector2 RotateVector(Vector2 vector, float rotation)
        {
            Vector2 rotatedVector = {
                cos(rotation) * vector.x - sin(rotation) * vector.y,
                cos(rotation) * vector.y + sin(rotation) * vector.x,
            };

            return rotatedVector;
        }

        /// @brief Adds two vectors
        /// @param vectorOne The initial vector
        /// @param vectorTwo The vector to add to the inital vector
        /// @returns The added vector
        inline static Vector2 AddVectors(Vector2 vectorOne, Vector2 vectorTwo)
        {
            return (Vector2){vectorOne.x + vectorTwo.x, vectorOne.y + vectorTwo.y};
        }

        /// @brief Subtracts the second vector from the first
        /// @param vectorOne The initial vector
        /// @param vectorTwo The vector to subtract from vectorTwo
        /// @returns The subtract vector
        inline static Vector2 SubtractVectors(Vector2 vectorOne, Vector2 vectorTwo)
        {
            return (Vector2){vectorOne.x - vectorTwo.x, vectorOne.y - vectorTwo.y};
        }

        /// @brief Multiplies two vectors
        /// @param vectorOne The initial vector
        /// @param vectorTwo The second vectors
        /// @returns The multiplied vector
        inline static Vector2 MultiplyVectors(Vector2 vectorOne, Vector2 vectorTwo)
        {
            return (Vector2){vectorOne.x * vectorTwo.x, vectorOne.y * vectorTwo.y};
        }

        /// @brief Multiplies a vector by a scalar value
        /// @param vector The initial vector
        /// @param scalar The scalar value to multiply the vector by
        /// @returns The scaled vector
        inline static Vector2 ScalarMultiplyVector(Vector2 vector, float scalar)
        {
            return (Vector2){vector.x * scalar, vector.y * scalar};
        }

        /// @brief Divides a vector by a scalar value
        /// @param vector The initial vector
        /// @param scalar The scalar value to divide the vector by
        /// @returns The scaled vector
        inline static Vector2 ScalarDivideVector(Vector2 vector, float scalar)
        {
            return (Vector2){vector.x / scalar, vector.y / scalar};
        }

        /// @brief Calculates the length (magnitude) of a vector
        /// @param vector The vector to calculate the length of
        /// @returns The vectors length
        inline static float LengthOf(Vector2 vector)
        {
            return sqrt(vector.x * vector.x + vector.y * vector.y);
        }

        /// @brief Linear interpolate between two vectors by a given weight
        /// @param start The starting vector
        /// @param end The ending vector
        /// @param weight The weight to interpolate by ([0,1] for interpolate, other values for extrapolation)
        /// @returns The interpolated vector
        inline static Vector2 LerpVector(Vector2 start, Vector2 end, float weight)
        {
            // (1.0f - weight)  * start + weight * end
            return AddVectors(ScalarMultiplyVector(start, (1.0f - weight)), ScalarMultiplyVector(end, weight));
        }

        /// @brief Normalizes the provided value
        /// @param vector The vector to normalized
        /// @returns The normalized vector
        inline static Vector2 Normalize(Vector2 vector)
        {
            float mag = LengthOf(vector);
            if (mag == 0)
            {
                return (Vector2){0, 0};
            }
            // Return normalized vector
            return ScalarDivideVector(vector, mag);
        }

        /// @brief Calculates the dot product of two (automatically normalized) vectors
        /// @param vectorOne The initial vector
        /// @param vectorTwo The second vector
        /// @returns The dot product of the two vectors
        inline static float Dot(Vector2 vectorOne, Vector2 vectorTwo)
        {
            Vector2 nOne = Normalize(vectorOne);
            Vector2 nTwo = Normalize(vectorTwo);

            return (nOne.x * nTwo.x) + (nOne.y + nTwo.y);
        }

    } VectorHelper;

    /// @brief A set of common math functions
    typedef struct MathHelper
    {
        /// @brief Linear interpolate between start and end values by some weight
        static inline float Lerp(float start, float end, float weight)
        {
            return (1.0f - weight)  * start + weight * end;
        }
    }MathHelper;

    typedef struct StaticTileMin
    {
        StaticTileMin(Vector2 size, Vector2 pos)
        {
            this->imageSize = size;
            this->imagePos = pos;
        }
        Vector2 imageSize;
        Vector2 imagePos;
    } StaticTileMin;
};

#endif