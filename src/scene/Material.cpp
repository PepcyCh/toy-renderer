#include "Material.h"

namespace toy_renderer {

bool Material::IsLight() const {
    return isLight;
}

void Material::Clear() {
    this->clear();
    isLight = false;
}

void Material::SetEmissive(const Spectrum &color) {
    isLight = true;
    emissive = color;
    if (emissive.GetLuminance() < LOWEST_LIGHT_LUMINANCE)
        isLight = false;
}

void Material::SetEmissive(Float r, Float g, Float b) {
    isLight = true;
    emissive = Spectrum(r, g, b);
    if (emissive.GetLuminance() < LOWEST_LIGHT_LUMINANCE)
        isLight = false;
}

Spectrum Material::GetEmissive() const {
    return emissive;
}

}