#pragma once

#include "Shader.h"
#include <vector>

namespace toy_renderer {

class FrameBuffer {
  public:
    void SetGeometry(int width, int height);
    std::pair<int, int> GetGeometry() const;

    void Use(bool retina = false) const;
    void DrawScreenQuad(const Shader &sh) const;

  protected:
    static struct ScreenVAO {
        ~ScreenVAO();

        void Construct();

        unsigned int VAO, VBO;
    } screenVAO;

    int width, height;
    unsigned int fboID = 0;
};

extern FrameBuffer defaultFrameBuffer;

}