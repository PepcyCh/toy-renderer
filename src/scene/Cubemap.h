#pragma once

#include "Shader.h"
#include "Camera.h"

namespace toy_renderer {

class Cubemap {
  public:
    Cubemap() = default;
    Cubemap(unsigned int ID) : ID(ID) {}
    Cubemap(const std::string &path, const std::string &suffix,
            bool gamma = false);

    void Draw(const Camera &cam, const Shader &sh) const;
    unsigned int GetID() const;

  private:
    static struct CubemapVAO {
        ~CubemapVAO();

        void Construct();

        unsigned int VAO, VBO;
    } cubemapVAO;

    unsigned int ID = 0;
};

}
