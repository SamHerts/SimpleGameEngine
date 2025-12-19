cmake_minimum_required(VERSION 3.16)
project(Game)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

set(SDLIMAGE_AVIF OFF)
set(SDLIMAGE_BMP OFF)
set(SDLIMAGE_GIF OFF)
set(SDLIMAGE_JPG OFF)
set(SDLIMAGE_JXL OFF)
set(SDLIMAGE_LBM OFF)
set(SDLIMAGE_PCX OFF)
set(SDLIMAGE_PNG ON)
set(SDLIMAGE_PNM OFF)
set(SDLIMAGE_QOI OFF)
set(SDLIMAGE_SVG ON)
set(SDLIMAGE_TGA OFF)
set(SDLIMAGE_TIF OFF)
set(SDLIMAGE_WEBP OFF)
set(SDLIMAGE_XCF OFF)
set(SDLIMAGE_XPM OFF)
set(SDLIMAGE_XV OFF)
add_subdirectory(vendored/SDL_image EXCLUDE_FROM_ALL)

#file(GLOB imgui_sources vendored/imgui/*.cpp vendored/imgui/*.h vendored/imgui/backends/imgui_impl_sdl3.cpp)

#add_library(imgui STATIC ${imgui_sources})

add_subdirectory(Math)
add_subdirectory(Components)
add_subdirectory(Core)
add_subdirectory(Systems)

# Create your game executable target as usual
add_executable(Game WIN32)

target_sources(Game PRIVATE main.cpp SDL_Includes.h)

#target_link_libraries(Game PRIVATE Systems Core Components Math imgui)
target_link_libraries(Game PRIVATE Systems Core Components Math)


# Link to the actual SDL3 library.
target_link_libraries(Game PRIVATE SDL3_image::SDL3_image SDL3::SDL3)

# Asset management
set(ASSETS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Assets")
set(ASSETS_OUT_DIR "${CMAKE_BINARY_DIR}/$<CONFIGURATION>/Assets")

add_custom_command(
        OUTPUT ${ASSETS_OUT_DIR}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${ASSETS_OUT_DIR}
        DEPENDS
        VERBATIM
)

add_custom_command(
        OUTPUT ${ASSETS_OUT_DIR}/dummy.txt # Dummy output to trigger the command
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${ASSETS_DIR} ${ASSETS_OUT_DIR}
        DEPENDS ${ASSETS_OUT_DIR}
        VERBATIM
)

add_custom_target(copy_assets ALL DEPENDS ${ASSETS_OUT_DIR}/dummy.txt)

add_dependencies(Game copy_assets)