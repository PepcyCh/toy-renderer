#pragma once

#include "Spectrum.h"
#include "Model.h"
#include "FrameBuffer.h"

namespace toy_renderer {

class Light {
  public:
    Light() : pos(), color(), shadow(false) {}
    Light(const Point3f &pos, const Spectrum &color) :
            pos(pos), color(color), shadow(false) {}

    void SetPosition(const Point3f &p);
    void SetColor(const Spectrum &color);
    void SetColor(Float r, Float g, Float b);

    void SwitchShadow(bool shadow);
    bool HasShadow() const;

    void SetToShader(const Shader &sh, int lightID) const;

  private:
    Point3f pos;
    Spectrum color;
    bool shadow;
};

}
