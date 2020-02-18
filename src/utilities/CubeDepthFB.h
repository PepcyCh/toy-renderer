#pragma once

#include "FrameBuffer.h"

namespace toy_renderer {

class CubeDepthFB : public FrameBuffer {
  public:
    CubeDepthFB(int width);

    void Clear();
    unsigned int GetColorAttachment() const;

  private:
    unsigned int color;
};

}
