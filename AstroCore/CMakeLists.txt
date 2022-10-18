add_library(astrocore 
engine/input.cpp
engine/file.cpp
engine/game.cpp
)

target_include_directories(${PROJECT_NAME} 
    PUBLIC ../header
    PUBLIC include
)

target_link_directories(${PROJECT_NAME}
    PRIVATE src
    PRIVATE lib
)