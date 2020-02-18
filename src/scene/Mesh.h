#pragma once

#include "geometry.h"
#include "Shader.h"
#include "Material.h"
#include <vector>

namespace toy_renderer {

class Mesh {
  public:
    Mesh() : VAO(0), VBO(), EBO(0), isSetup(false) {}
    ~Mesh() { Clear(); }

    void UpdatePositions(const std::vector<Point3f> &vec);
    void UpdateNormals(const std::vector<Normal3f> &vec);
    void UpdateUVs(const std::vector<Point2f> &vec);
    void UpdateTangents(const std::vector<Vector3f> &vec);
    void UpdateBitangents(const std::vector<Vector3f> &vec);
    void UpdateIndices(const std::vector<unsigned int> &vec);
    void UpdateMaterial(const Material &vec);

    bool IsValid() const;
    size_t Size() const;
    bool Empty() const;
    void Clear();

    Point3f GetCenter() const;
    void CalcTangents();

    void SetUp() const;
    void Draw(const Shader &sh) const;

  private:
    std::vector<Point3f> position;
    std::vector<Normal3f> normals;
    std::vector<Point2f> uvs;
    std::vector<Vector3f> tangents;
    std::vector<Vector3f> bitangents;
    std::vector<unsigned int> indices;
    Material textures;
    mutable std::vector<float> bufferData;
    mutable unsigned int VAO, VBO, EBO;
    mutable bool isSetup;
};

}