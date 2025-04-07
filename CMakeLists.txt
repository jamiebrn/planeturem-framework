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

# find_package(Freetype REQUIRED)
FetchContent_Declare(
    freetype
    GIT_REPOSITORY https://gitlab.freedesktop.org/freetype/freetype
    GIT_TAG VER-2-13-3
)
FetchContent_MakeAvailable(freetype)

FetchContent_Declare(
    ogg
    GIT_REPOSITORY https://github.com/xiph/ogg
    GIT_TAG v1.3.5
)
FetchContent_MakeAvailable(ogg)

FetchContent_Declare(
    vorbis
    GIT_REPOSITORY https://github.com/xiph/vorbis
    GIT_TAG v1.3.7
)

set(OGG_INCLUDE_DIR ${ogg_SOURCE_DIR}/include)
set(OGG_LIBRARY ogg)

FetchContent_MakeAvailable(vorbis)

include_directories(include/)
include_directories(${freetype_SOURCE_DIR}/include)
include_directories(${ogg_SOURCE_DIR}/include)
include_directories(${vorbis_SOURCE_DIR}/include)

file(GLOB_RECURSE CPP_SRC_FILES src/*.cpp src/*.c)

add_executable(GLFramework ${CPP_SRC_FILES})
# target_link_libraries(GLFramework PRIVATE SDL2::SDL2main)
target_link_libraries(GLFramework PRIVATE SDL2::SDL2)
target_link_libraries(GLFramework PRIVATE vorbis vorbisfile)
target_link_libraries(GLFramework PRIVATE freetype)
target_compile_features(GLFramework PRIVATE cxx_std_20)

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Data
)
add_dependencies(GLFramework copy_assets)