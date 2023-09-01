add_library(astrocore
engine/input.cpp
entities/baseEntities/entity.cpp
entities/baseEntities/animatedSpriteEntity.cpp
entities/baseEntities/spriteEntity.cpp
engine/scene.cpp
engine/texturemanager.cpp
entities/baseEntities/light2D.cpp
entities/baseEntities/tilemap.cpp
engine/filemanager.cpp
entities/baseEntities/tilemapLayer.cpp
entities/physicsEntities/physicsEntity.cpp
engine/physicsDebug.cpp
entities/physicsEntities/trigger2D.cpp
engine/collisionTracker.cpp
)
target_include_directories(${PROJECT_NAME}
    PRIVATE include
    PRIVATE include/tmxlite
    PUBLIC engine
    PUBLIC entities
)

target_link_directories(${PROJECT_NAME}
    PRIVATE lib
    PRIVATE engine
    PRIVATE entities
)