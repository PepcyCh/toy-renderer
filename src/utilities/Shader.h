#pragma once

#include "geometry.h"
#include "Spectrum.h"
#include <string>

namespace toy_renderer {

class Shader {
  public:
    Shader(const std::string &vsFile, const std::string &fsFile);
    Shader(const std::string &vsFile, const std::string &fsFile,
            const std::string &gsFile);

    bool IsValid() const;

    void Use() const;

    void SetInt(const std::string &name, int value) const;
    void SetFloat(const std::string &name, float value) const;
    void SetVec3(const std::string &name, const Vector3f &v) const;
    void SetVec3(const std::string &name, const Point3f &p) const;
    void SetVec3(const std::string &name, const Normal3f &n) const;
    void SetVec3(const std::string &name, const Spectrum &s) const;
    void SetMat4(const std::string &name, const Matrix44 &m) const;

  private:
    unsigned int ID;
};

}