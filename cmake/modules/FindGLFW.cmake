# GLFW static library
find_library(GLFW_LIBRARIES
    NAMES libglfw3.a
    PATHS ${PROJECT_SOURCE_DIR}/src/external/glfw
    )

# GLFW library dir
find_path(GLFW_LIBRARY_DIRS
    NAMES libglfw3.a
    PATHS ${PROJECT_SOURCE_DIR}/src/external/glfw
    )

# GLFW include dir
find_path(GLFW_INCLUDE_DIRS
    NAMES glfw3.h
    PATHS ${PROJECT_SOURCE_DIR}/src/external/glfw/GLFW
    )

# Version
set(GLFW_VERSION 3.3.2)

# Set package standard args
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLFW 
    REQUIRED_VARS GLFW_LIBRARIES GLFW_INCLUDE_DIRS GLFW_LIBRARY_DIRS
    VERSION_VAR GLFW_VERSION)
