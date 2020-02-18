#pragma once

#include "Spectrum.h"
#include "Texture.h"
#include <vector>

namespace toy_renderer {

class Material : public std::vector<Texture> {
  public:
    bool IsLight() const;
    void Clear();

    void SetEmissive(const Spectrum &color);
    void SetEmissive(Float r, Float g, Float b);
    Spectrum GetEmissive() const;

    static constexpr double LOWEST_LIGHT_LUMINANCE = 0.01;

  private:
    Spectrum emissive;
    bool isLight = false;
};

}