# Assimp dynamic library
find_library(Assimp_LIBRARIES
    NAMES libassimp.dylib
    PATHS ${PROJECT_SOURCE_DIR}/src/external/assimp
    )

# Assimp library dir
find_path(Assimp_LIBRARY_DIRS
    NAMES libassimp.dylib
    PATHS ${PROJECT_SOURCE_DIR}/src/external/assimp
    )

# Assimp include dir
find_path(Assimp_INCLUDE_DIRS
    NAMES assimp
    PATHS ${PROJECT_SOURCE_DIR}/src/external/assimp/include
    )

# Version
set(Assimp_VERSION 5.0.0)

# Set package standard args
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Assimp
    REQUIRED_VARS Assimp_LIBRARIES Assimp_INCLUDE_DIRS Assimp_LIBRARY_DIRS
    VERSION_VAR Assimp_VERSION)
