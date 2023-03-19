add_library(astrocore
src/input.cpp
src/file.cpp
src/entity.cpp
src/animatedSpriteEntity.cpp
src/spriteEntity.cpp
)

target_include_directories(${PROJECT_NAME} 
    PUBLIC header
    PUBLIC include
)

target_link_directories(${PROJECT_NAME}
    PRIVATE src
    PRIVATE lib
)