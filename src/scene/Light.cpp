#include "Light.h"

namespace toy_renderer {

void Light::SetPosition(const Point3f &p) {
    pos = p;
}

void Light::SetColor(const Spectrum &color) {
    this->color = color;
}

void Light::SetColor(Float r, Float g, Float b) {
    color = Spectrum(r, g, b);
}
void Light::SwitchShadow(bool on) {
    shadow = on;
}

bool Light::HasShadow() const {
    return shadow;
}

void Light::SetToShader(const Shader &sh, int lightID) const {
    std::string strID = std::to_string(lightID);
    sh.SetVec3("lightPos[" + strID + "]", pos);
    sh.SetVec3("lightColor[" + strID + "]", color);
}

}