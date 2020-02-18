#pragma once

#include "Mesh.h"

namespace toy_renderer {

class MeshUtil {
  public:
    static Mesh Cube();
    static Mesh Sphere();

    static unsigned int CubeVAO();
};

}
