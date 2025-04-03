cmake_minimum_required(VERSION 3.16)
project(GLFramework LANGUAGES CXX C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

include(FetchContent)
FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.30.5
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(SDL2)

# FetchContent_Declare(
#     SDL2_image
#     GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
#     GIT_TAG release-2.8.4
#     GIT_SHALLOW TRUE
#     GIT_PROGRESS TRUE
# )
# FetchContent_MakeAvailable(SDL2_image)

# FetchContent_Declare(
#     SDL2_ttf
#     GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
#     GIT_TAG release-2.22.0
#     GIT_SHALLOW TRUE
#     GIT_PROGRESS TRUE
# )
# FetchContent_MakeAvailable(SDL2_ttf)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm
    GIT_TAG 1.0.1
)
FetchContent_MakeAvailable(glm)

# find_package(Freetype REQUIRED)
FetchContent_Declare(
    freetype
    GIT_REPOSITORY https://gitlab.freedesktop.org/freetype/freetype
    GIT_TAG VER-2-13-3
)
FetchContent_MakeAvailable(freetype)

include_directories(include/)
include_directories(${glm_SOURCE_DIR})
include_directories(${freetype_SOURCE_DIR}/include)

file(GLOB_RECURSE CPP_SRC_FILES src/*.cpp src/*.c include/*.c)

add_executable(GLFramework ${CPP_SRC_FILES})
# target_link_libraries(GLFramework PRIVATE SDL2::SDL2main)
target_link_libraries(GLFramework PRIVATE SDL2::SDL2)
target_link_libraries(GLFramework PRIVATE freetype)
# target_include_directories(GLFramework PRIVATE ${freetype_INCLUDE_DIRS})
# target_link_libraries(GLFramework PRIVATE SDL2_image::SDL2_image)
# target_link_libraries(GLFramework PRIVATE SDL2_ttf::SDL2_ttf)
target_compile_features(GLFramework PRIVATE cxx_std_20)

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Data
)
add_dependencies(GLFramework copy_assets)