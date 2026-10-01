cmake_minimum_required(VERSION 3.20)

project(ShadowFight)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
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
    # src/Mesh.cpp
    # src/Model.cpp
    ${ANIMATION_SOURCES}
    ${RENDERER_SOURCES}
)

target_include_directories(ShadowFight PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(ShadowFight PRIVATE
    OpenGL::GL
    glfw
    glad::glad
    assimp::assimp
    glm::glm
)