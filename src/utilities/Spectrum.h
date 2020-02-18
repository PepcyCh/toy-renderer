#pragma once

#include "defines.h"

namespace toy_renderer {

class Spectrum {
  public:
    Spectrum() : r(0), g(0), b(0) {}
    Spectrum(Float r, Float g, Float b) : r(r), g(g), b(b) {}

    Float GetLuminance() const {
        return 0.299 * r + 0.587 * g + 0.114 * b;
    }

    Float r, g, b;
};

}
