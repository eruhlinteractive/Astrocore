add_library(astrocore 
input.cpp
file.cpp
entity.cpp
spriteEntity.cpp
)

target_include_directories(${PROJECT_NAME} 
    PUBLIC ../headers
)