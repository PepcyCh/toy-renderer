# stb include dir
set(STB_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src/external/stb)

# Set package standard args
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(STB
    REQUIRED_VARS STB_INCLUDE_DIRS
    )
