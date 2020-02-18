#pragma once

#include "FrameBuffer.h"

namespace toy_renderer {

class DeferredRenderingFB : public FrameBuffer {
public:
    DeferredRenderingFB(int width, int height);

    void Clear();
    void SetToShader(const Shader &sh, int offset = 0) const;

private:
    unsigned int colors[5];
    unsigned int depth;
};

}
