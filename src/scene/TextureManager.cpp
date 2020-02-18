#include "glad/glad.h"
#include "defines.h"
#include "TextureManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace toy_renderer {

TextureManager gTextureManager;

static void GetFormat(int nn, GLenum *format, GLenum *iformat, bool gamma) {
    if (nn == 1) {
        *format = GL_RED;
        *iformat = GL_RED;
    } else if (nn == 3) {
        *format = GL_RGB;
        *iformat = GL_SRGB;
    } else if (nn == 4) {
        *format = GL_RGBA;
        *iformat = GL_SRGB_ALPHA;
    } else {
        format = nullptr;
        iformat = nullptr;
    }
    if (!gamma)
        *iformat = *format;
}

TextureManager::~TextureManager() {
    for (const auto &p : texMap) {
        glDeleteTextures(1, &p.second);
    }
}

unsigned int TextureManager::LoadTextureFromFile(const std::string &path,
        bool gamma) {
    if (auto it = texMap.find(path); it != texMap.end())
        return it->second;

    int nx, ny, nn;
    unsigned char *data = stbi_load(path.c_str(), &nx, &ny, &nn, 0);
    if (!data)
        return 0;

    GLenum format, iformat;
    GetFormat(nn, &format, &iformat, gamma);
    if (!format) {
        stbi_image_free(data);
        return 0;
    }

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, iformat, nx, ny, 0, format,
            GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
            GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
    texMap[path] = texture;
    return texture;
}

unsigned int TextureManager::LoadCubemapFromFile(const std::string &path,
        const std::string &suffix, bool gamma) {
    if (auto it = cubeMap.find(path + suffix); it != cubeMap.end())
        return it->second;

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    for (int i = 0; i < 6; i++) {
        static const char *cubemap_name[6] = {
            "right", "left", "top",
            "bottom", "front", "back"
        };
        int nx, ny, nn;
        unsigned char *data = stbi_load(
                (path + cubemap_name[i] + suffix).c_str(), &nx, &ny, &nn, 0);
        if (!data)
            return 0;

        GLenum format, iformat;
        GetFormat(nn, &format, &iformat, gamma);
        if (!format) {
            stbi_image_free(data);
            glDeleteTextures(1, &texture);
            return 0;
        }

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, iformat,
                nx, ny, 0, format, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    cubeMap[path + suffix] = texture;
    return texture;
}

unsigned int TextureManager::LoadHDRFromFile(const std::string &path) {
    if (auto it = texMap.find(path); it != texMap.end())
        return it->second;

    int nx, ny, nn;
    stbi_set_flip_vertically_on_load(true);
    float *data = stbi_loadf(path.c_str(), &nx, &ny, &nn, 0);
    if (!data)
        return 0;
    stbi_set_flip_vertically_on_load(false);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, nx, ny, 0, GL_RGB,
                 GL_FLOAT, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
    texMap[path] = texture;
    return texture;
}

}