add_library(astrocore 
input.cpp
file.cpp
entity.cpp
spriteEntity.cpp
)

target_include_directories(${PROJECT_NAME} 
    PUBLIC ../header
    PUBLIC include
)

target_link_directories(${PROJECT_NAME}
    PRIVATE src
    PRIVATE lib
)