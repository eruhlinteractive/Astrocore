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
entities/baseEntities/stackedSpriteEntity.cpp
entities/physicsEntities/physicsEntity.cpp
engine/physicsDebug.cpp
entities/physicsEntities/trigger2D.cpp
engine/collisionTracker.cpp
entities/baseEntities/shapeEntity.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC engine
    PUBLIC entities
    PRIVATE /home/ethan/vcpkg/installed/x64-linux/include
)

target_link_directories(${PROJECT_NAME}
    PRIVATE engine
    PRIVATE entities
    PRIVATE /home/ethan/vcpkg/installed/x64-linux/lib/
)