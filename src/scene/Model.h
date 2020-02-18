#pragma once

#include "Mesh.h"

namespace toy_renderer {

class Model {
  public:
    Model() : meshes(), model() {}
    Model(const Mesh &mesh, const Matrix44 &mat = Matrix44());
    ~Model() { Clear(); }

    bool IsValid() const;
    bool Empty() const;
    void Clear();

    void UpdateModelMatrix(const Matrix44 &m);
    void AddMesh(const Mesh &mesh);

    Point3f GetCenter() const;

    void Draw(const Shader &sh) const;

  private:
    std::vector<Mesh> meshes;
    Matrix44 model;
};

}