cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME toy-renderer)
project(${PROJECT_NAME})

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules/")

configure_file(
    ${PROJECT_SOURCE_DIR}/src/defines.h.in
    ${PROJECT_SOURCE_DIR}/src/defines.h
    )

# OS X Frameworks
if(APPLE)
    include_directories( "/System/Library/Frameworks" )
    find_library(COCOA_LIBRARIES Cocoa)
    find_library(IOKIT_LIBRARIES IOkit)
    find_library(COREVIDEO_LIBRARIES CoreVideo)
endif()

# compile flags
set(CMAKE_CXX_FLAGS "-std=c++17 -Wno-int-to-void-pointer-cast")

# packages
find_package(OpenGL REQUIRED)
find_package(GLFW REQUIRED)
find_package(GLAD REQUIRED)
find_package(STB REQUIRED)

add_subdirectory(src)
