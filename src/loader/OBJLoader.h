#pragma once

#include "Scene.h"
#include "Material.h"
#include <map>

namespace toy_renderer {

class OBJLoader {
  public:
    Scene ReadFile(const std::string &filename, bool calcTangents = false);

  private:
    void Clear();
    void ReadMTLFile(const std::string &filename);
    void Triangulate(const std::vector<uint> &vec);

    void AddMeshToModel();
    void AddModelToScene();
    void AddMaterialToMap(const std::string &name, bool has_name);

    static constexpr int BUF_LEN = 4096;
    char buf[BUF_LEN];

    std::string directory;

    std::vector<Point3f> v, v_buf;
    std::vector<Normal3f> vn, vn_buf;
    std::vector<Point2f> vt, vt_buf;
    std::vector<uint> f, f_buf;
    std::map<std::string, Material> materials;
    std::map<std::tuple<int, int, int>, unsigned int> id_map;

    Scene scene_buf;
    Model model_buf;
    Mesh mesh_buf;
    Material material_buf;

    bool calcTangents;
};

}