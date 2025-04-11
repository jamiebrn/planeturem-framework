cmake_minimum_required(VERSION 3.16)
project(PlaneturemFramework LANGUAGES CXX C)

include(FetchContent)
FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.30.5
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(SDL2)

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

file(GLOB_RECURSE CPP_SRC_FILES src/*.cpp src/*.c)

add_library(PlaneturemFramework ${CPP_SRC_FILES})

target_include_directories(PlaneturemFramework PRIVATE include/)
target_include_directories(PlaneturemFramework PRIVATE ${freetype_SOURCE_DIR}/include)
target_include_directories(PlaneturemFramework PRIVATE ${ogg_SOURCE_DIR}/include)
target_include_directories(PlaneturemFramework PRIVATE ${vorbis_SOURCE_DIR}/include)

target_link_libraries(PlaneturemFramework PRIVATE SDL2::SDL2)
target_link_libraries(PlaneturemFramework PRIVATE vorbis vorbisfile)
target_link_libraries(PlaneturemFramework PRIVATE freetype)
target_compile_features(PlaneturemFramework PRIVATE cxx_std_20)