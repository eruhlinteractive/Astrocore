add_library(astrocore 
input.cpp
file.cpp
)

target_include_directories(${PROJECT_NAME} 
    PUBLIC ../headers
)