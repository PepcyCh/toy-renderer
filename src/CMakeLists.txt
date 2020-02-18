# include directories
set(TR_INCLUDE_DIRS
    .
    geometry
    loader
    window
    utilities
    scene
    )

include_directories(
    ${TR_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
    ${GLAD_INCLUDE_DIRS}
    ${STB_INCLUDE_DIRS}
    )

link_directories(
    ${GLFW_LIBRARY_DIRS}
    )

# source files
set(TR_GEOMETRY_SRC
    geometry/geometry.h
    geometry/Matrix44.cpp
    geometry/Matrix44.h
    geometry/Normal3.h
    geometry/Point2.h
    geometry/Point3.h
    geometry/Vector2.h
    geometry/Vector3.h
    )

set(TR_LOADER_SRC
    loader/OBJLoader.cpp
    loader/OBJLoader.h
    )

set(TR_WINDOW_SRC
    window/InputManager.cpp
    window/InputManager.h
    window/Window.cpp
    window/Window.h
    )

set(TR_UTILITIES_SRC
    utilities/Camera.cpp
    utilities/Camera.h
    utilities/FrameBuffer.cpp
    utilities/FrameBuffer.h
    utilities/Shader.cpp
    utilities/Shader.h
    utilities/Spectrum.h
    utilities/DeferredRenderingFB.cpp
    utilities/DeferredRenderingFB.h
    utilities/CubeDepthFB.cpp
    utilities/CubeDepthFB.h
    utilities/OffScreenFB.cpp
    utilities/OffScreenFB.h
    utilities/SSAOFB.cpp
    utilities/SSAOFB.h
    utilities/CubeFB.cpp utilities/CubeFB.h)

set(TR_SCENE_SRC
    scene/Cubemap.cpp
    scene/Cubemap.h
    scene/Light.cpp
    scene/Light.h
    scene/Material.cpp
    scene/Material.h
    scene/Mesh.cpp
    scene/Mesh.h
    scene/Model.cpp
    scene/Model.h
    scene/Scene.cpp
    scene/Scene.h
    scene/Texture.cpp
    scene/Texture.h
    scene/TextureManager.cpp
    scene/TextureManager.h
    scene/MeshUtil.cpp
    scene/MeshUtil.h
    )

set(TR_SRC
    main.cpp
    defines.h
    ${TR_GEOMETRY_SRC}
    ${TR_LOADER_SRC}
    ${TR_WINDOW_SRC}
    ${TR_UTILITIES_SRC}
    ${TR_SCENE_SRC}
    ${GLAD_SOURCES}
    )

# executable
add_executable(${PROJECT_NAME} ${TR_SRC})
target_link_libraries(${PROJECT_NAME}
    ${GLFW_LIBRARIES}
    )
if(APPLE)
    target_link_libraries(${PROJECT_NAME}
        ${COCOA_LIBRARIES}
        ${IOKIT_LIBRARIES}
        ${COREVIDEO_LIBRARIES}
        )
endif()
