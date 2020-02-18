#pragma once

#include "FrameBuffer.h"
#include "geometry.h"

namespace toy_renderer {

class SSAOFB : public FrameBuffer {
  public:
    SSAOFB(int width, int height);
    ~SSAOFB() { Clear(); }

    void Clear();
    unsigned int GetColorAttachment() const;
    void SetToShader(const Shader &sh) const;

  private:
    std::vector<Vector3f> samples, noises;
    unsigned int color;
    unsigned int noisesTex;
    static constexpr int MAX_SAMPLE = 64;
};

}
