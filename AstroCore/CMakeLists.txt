add_library(astrocore
src/input.cpp
src/entity.cpp
src/animatedSpriteEntity.cpp
src/spriteEntity.cpp
src/scene.cpp
src/texturemanager.cpp
src/light2D.cpp
src/tilemap.cpp
src/filemanager.cpp
src/tilemapLayer.cpp
src/physicsEntity.cpp
src/physicsDebug.cpp
)
target_include_directories(${PROJECT_NAME}
    PRIVATE include
    PRIVATE include/tmxlite
    PUBLIC header
)

target_link_directories(${PROJECT_NAME}
    PRIVATE lib
    PRIVATE src
)