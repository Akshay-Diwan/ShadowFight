cmake_minimum_required(VERSION 3.20)

project(ShadowFight)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

file(GLOB_RECURSE ANIMATION_SOURCES
    "${CMAKE_SOURCE_DIR}/src/animation/*.cpp"
)

file(GLOB_RECURSE RENDERER_SOURCES
    "${CMAKE_SOURCE_DIR}/src/renderer/*.cpp"
)

add_executable(ShadowFight
    src/main.cpp
    src/gl.c
    src/PlayerState.cpp
    src/CharacterAnimation.cpp
    ${ANIMATION_SOURCES}
    ${RENDERER_SOURCES}
)

target_include_directories(ShadowFight PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party
)

target_link_libraries(ShadowFight PRIVATE
    OpenGL::GL
    glfw
    assimp::assimp
    glm::glm
)