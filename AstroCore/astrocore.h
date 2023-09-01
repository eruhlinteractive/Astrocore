// Single file to include functionality of the Astro Core

#include "include/raylib.h"
#include "game.h"

// Data
#include "data/structs.h"
#include "data/interfaces.h"

// Base entities
#include "entities/baseEntities/tilemap.h"
#include "entities/baseEntities/pixelPerfectCamera.h"
#include "entities/baseEntities/cameraEntity.h"
#include "entities/baseEntities/light2D.h"
#include "entities/baseEntities/animatedSpriteEntity.h"
#include "entities/baseEntities/entity.h"

// Physics entities
#include "entities/physicsEntities/physicsEntity.h"
#include "entities/physicsEntities/trigger2D.h"

// Engine
#include "engine/scene.h"
#include "engine/texturemanager.h"
#include "engine/input.h"
#include "engine/filemanager.h"


#include "engine/debug.h"

