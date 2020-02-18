#pragma once

#include "FrameBuffer.h"
#include "Scene.h"

namespace toy_renderer {

class CubeFB : public FrameBuffer {
  public:
    CubeFB(int width, bool isHDR = false);

    void Clear();
    unsigned int GetColorAttachment() const;
    void DrawToBuffer(const Scene &scene, const Shader &sh,
                      const Point3f &center = Point3f()) const;
    void DrawToBuffer(const Scene &scene, const Shader &sh,
                      const Shader &shCube,
                      const Point3f &center = Point3f()) const;
    void DrawToBuffer(const Shader &sh) const;
    void DrawToBuffer(const Shader &sh, int mip) const;
    void FromEquirectangular(const std::string &path, const Shader &sh) const;

  private:
    unsigned int color;
    unsigned int depth;
};

}
