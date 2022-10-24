cmake_minimum_required(VERSION 3.0.0)
project(FarmingGame VERSION 0.1.0)

include(CTest)
enable_testing()


add_executable(FarmingGame main.cpp src/game.cpp)
add_subdirectory(AstroCore)

target_include_directories(${PROJECT_NAME} 
    PUBLIC include
    PUBLIC header
)


target_link_directories(${PROJECT_NAME}
    PRIVATE src
    PRIVATE lib
    PRIVATE res
 )

target_link_libraries(${PROJECT_NAME} raylib astrocore)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
