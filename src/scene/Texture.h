#pragma once

#include "Spectrum.h"
#include <string>

namespace toy_renderer {

class Texture {
  public:
    enum TextureType {
        AMBIENT,
        AMBIENT_OCCLUSION,
        ALBEDO,
        SPECULAR,
        NORMAL,
        METALLIC,
        ROUGHNESS,
        EXPONENT
    };

    Texture(TextureType type, Float r, Float g, Float b);
    Texture(TextureType type, const std::string &path, bool gamma = false);

    std::string GetTypeString() const;

    TextureType type = ALBEDO;
    bool isImg = false;
    unsigned int imgID;
    Spectrum color;

  private:
    void LoadTextureFromFile(const std::string &path, bool gamma);
};

}