#include "Model.h"

namespace toy_renderer {

Model::Model(const Mesh &mesh, const Matrix44 &mat) : model(mat) {
    meshes = std::vector<Mesh>({mesh});
}

bool Model::IsValid() const {
    for (const Mesh &mesh : meshes) {
        if (!mesh.IsValid())
            return false;
    }
    return true;
}

bool Model::Empty() const {
    return meshes.empty();
}

void Model::Clear() {
    meshes.clear();
    model = Matrix44();
}

void Model::UpdateModelMatrix(const Matrix44 &m) {
    model = m;
}

void Model::AddMesh(const Mesh &mesh) {
    meshes.push_back(mesh);
}

Point3f Model::GetCenter() const {
    Point3f center;
    size_t N = 0;
    for (const Mesh &m : meshes) {
        center += m.GetCenter() * m.Size();
        N += m.Size();
    }
    return center / N;
}

void Model::Draw(const Shader &sh) const {
    sh.SetMat4("model", model);
    sh.SetMat4("model_TI", Transpose(Inverse(model)));
    for (const Mesh &mesh : meshes) {
        mesh.Draw(sh);
    }
}

}
