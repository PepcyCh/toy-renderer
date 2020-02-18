#include "Texture.h"
#include "TextureManager.h"

namespace toy_renderer {

Texture::Texture(TextureType type, Float r, Float g, Float b) : type(type) {
    isImg = false;
    color = Spectrum(r, g, b);
}

Texture::Texture(TextureType type, const std::string &path, bool gamma)
        : type(type) {
    isImg = true;
    LoadTextureFromFile(path, gamma);
}

std::string Texture::GetTypeString() const {
    std::string str;
    if (type == AMBIENT)
        str = "ambient";
    else if (type == AMBIENT_OCCLUSION)
        str = "ao";
    else if (type == ALBEDO)
        str = "albedo";
    else if (type == SPECULAR)
        str = "specular";
    else if (type == NORMAL)
        str = "normal";
    else if (type == METALLIC)
        str = "metallic";
    else if (type == ROUGHNESS)
        str = "roughness";
    else if (type == EXPONENT)
        str = "exponent";
    return str;
}

void Texture::LoadTextureFromFile(const std::string &path, bool gamma) {
    imgID = gTextureManager.LoadTextureFromFile(path, gamma);
    if (imgID == 0) {
        isImg = false;
        color = Spectrum();
    }
}

}