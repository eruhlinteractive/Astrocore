cmake_minimum_required(VERSION 3.0.0)
project(FarmingGame VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(FarmingGame main.cpp src/game.cpp)

target_include_directories(${PROJECT_NAME} 
    PUBLIC include
    PUBLIC EngineCore
    PUBLIC header
)


target_link_directories(${PROJECT_NAME}
    PRIVATE src
    PUBLIC EngineCore
 )

target_link_libraries(${PROJECT_NAME} raylib)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
