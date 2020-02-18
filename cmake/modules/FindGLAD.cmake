# glad include dir
set(GLAD_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src/external/glad/include)

# glad sources
set(GLAD_SOURCES ${PROJECT_SOURCE_DIR}/src/external/glad/src/glad.c)

# Set package standard args
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLAD
    REQUIRED_VARS GLAD_INCLUDE_DIRS GLAD_SOURCES
    )
