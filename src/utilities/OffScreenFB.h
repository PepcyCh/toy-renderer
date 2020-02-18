#pragma once

#include "FrameBuffer.h"

namespace toy_renderer {

class OffScreenFB : public FrameBuffer {
  public:
    OffScreenFB(int width, int height);

    void Clear();
    unsigned int GetColorAttachment() const;

  private:
    unsigned int color;
    unsigned int depth;
};

}
