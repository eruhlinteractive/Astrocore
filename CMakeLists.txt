cmake_minimum_required(VERSION 3.0.0)
project(FarmingGame.x86_64 VERSION 0.1.0)

include(CTest)
enable_testing()


add_executable(FarmingGame.x86_64 main.cpp)
add_subdirectory(AstroCore)

target_include_directories(${PROJECT_NAME} 
    PUBLIC include
    PUBLIC header
)

target_link_directories(${PROJECT_NAME}
    PRIVATE src
    PRIVATE lib
 )

target_link_libraries(${PROJECT_NAME} raylib astrocore)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# Copy relevant directories/files into bin folder for distribution
add_custom_command(TARGET ${PROJECT_NAME} COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res ${CMAKE_SOURCE_DIR}/build/res)
#add_custom_command(TARGET ${PROJECT_NAME} COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/build/FarmingGame.x86_64 ${CMAKE_SOURCE_DIR}/build/bin)