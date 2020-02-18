#pragma once

#include <string>
#include <map>

namespace toy_renderer {

class TextureManager {
  public:
    ~TextureManager();

    unsigned int LoadTextureFromFile(const std::string &path,
                                     bool gamma = false);
    unsigned int LoadCubemapFromFile(const std::string &path,
                                     const std::string &suffix,
                                     bool gamma = false);
    unsigned int LoadHDRFromFile(const std::string &path);

  private:
    std::map<std::string, unsigned int> texMap;
    std::map<std::string, unsigned int> cubeMap;
};

extern TextureManager gTextureManager;

}