#include "glad/glad.h"
#include "Mesh.h"

namespace toy_renderer {

void Mesh::UpdatePositions(const std::vector<Point3f> &vec) {
    position = vec;
    isSetup = false;
}

void Mesh::UpdateNormals(const std::vector<Normal3f> &vec) {
    normals = vec;
    isSetup = false;
}

void Mesh::UpdateUVs(const std::vector<Point2f> &vec) {
    uvs = vec;
    isSetup = false;
}

void Mesh::UpdateTangents(const std::vector<Vector3f> &vec) {
    tangents = vec;
    isSetup = false;
}

void Mesh::UpdateBitangents(const std::vector<Vector3f> &vec) {
    bitangents = vec;
    isSetup = false;
}

void Mesh::UpdateIndices(const std::vector<unsigned int> &vec) {
    indices = vec;
    isSetup = false;
}

void Mesh::UpdateMaterial(const Material &vec) {
    textures = vec;
}

bool Mesh::IsValid() const {
    size_t N = position.size();

    if (!normals.empty() && normals.size() != N)
        return false;
    if (!uvs.empty() && uvs.size() != N)
        return false;
    size_t szT = tangents.size();
    size_t szB = bitangents.size();
    if (szT != 0 && szT != N)
        return false;
    if (szB != 0 && szB != N)
        return false;
    if (szT != szB)
        return false;

    return indices.size() % 3 == 0;
}

size_t Mesh::Size() const {
    return position.size();
}

bool Mesh::Empty() const {
    return position.empty();
}

void Mesh::Clear() {
    position.clear();
    normals.clear();
    uvs.clear();
    tangents.clear();
    bitangents.clear();
    indices.clear();
    textures.Clear();
    glDeleteVertexArrays(1, &VAO);
    VAO = 0;
    isSetup = false;
}

Point3f Mesh::GetCenter() const {
    Point3f center;
    for (const Point3f &p : position) center += p;
    return center / position.size();
}

void Mesh::CalcTangents() {
    tangents.clear();
    bitangents.clear();
    if (!IsValid())
        return;

    size_t N = position.size();
    tangents.resize(N);
    bitangents.resize(N);
    std::vector<int> deg(N, 0);

    for (int i = 0; i < indices.size(); i += 3) {
        Vector3f e1 = position[indices[i + 1]] - position[indices[i]];
        Vector3f e2 = position[indices[i + 2]] - position[indices[i]];
        Vector2f u1 = uvs[indices[i + 1]] - uvs[indices[i]];
        Vector2f u2 = uvs[indices[i + 2]] - uvs[indices[i]];
        Float f = 1.0 / Cross(u1, u2);

        Vector3f t = Vector3f(f * (u2.y * e1.x - u1.y * e2.x),
                              f * (u2.y * e1.y - u1.y * e2.y),
                              f * (u2.y * e1.z - u1.y * e2.z));
        t = Normalize(t);
        Vector3f b = Vector3f(f * (u1.x * e2.x - u2.x * e1.x),
                              f * (u1.x * e2.y - u2.x * e1.y),
                              f * (u1.x * e2.z - u2.x * e1.z));
        b = Normalize(b);

        tangents[indices[i]] += t;
        bitangents[indices[i]] += b;
        ++deg[indices[i]];
        tangents[indices[i + 1]] += t;
        bitangents[indices[i + 1]] += b;
        ++deg[indices[i + 1]];
        tangents[indices[i + 2]] += t;
        bitangents[indices[i + 2]] += b;
        ++deg[indices[i + 2]];
    }

    for (int i = 0; i < N; i++) {
        Float inv = 1.0 / deg[i];
        tangents[i] = Normalize(tangents[i] * inv);
        bitangents[i] = Normalize(bitangents[i] * inv);
    }

    isSetup = false;
}

void Mesh::SetUp() const {
    if (Empty() || !IsValid())
        return;

    size_t N = position.size();
    size_t szVert = 3 + (normals.empty() ? 0 : 3) + (uvs.empty() ? 0 : 2) +
            (tangents.empty() ? 0 : 6);
    size_t bufferDataSize = szVert * N;
    bufferData.resize(bufferDataSize);

    for (int i = 0, j = 0; i < N; i++) {
        bufferData[j] = position[i].x;
        bufferData[j + 1] = position[i].y;
        bufferData[j + 2] = position[i].z;
        j += 3;
        if (!normals.empty()) {
            bufferData[j] = normals[i].x;
            bufferData[j + 1] = normals[i].y;
            bufferData[j + 2] = normals[i].z;
            j += 3;
        }
        if (!uvs.empty()) {
            bufferData[j] = uvs[i].x;
            bufferData[j + 1] = uvs[i].y;
            j += 2;
        }
        if (!tangents.empty()) {
            bufferData[j] = tangents[i].x;
            bufferData[j + 1] = tangents[i].y;
            bufferData[j + 2] = tangents[i].z;
            j += 3;
            bufferData[j] = bitangents[i].x;
            bufferData[j + 1] = bitangents[i].y;
            bufferData[j + 2] = bitangents[i].z;
            j += 3;
        }
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 indices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, bufferDataSize * sizeof(float),
            bufferData.data(), GL_STATIC_DRAW);

    int nAttrib = 0, offset = 0;
    szVert *= sizeof(float);
    // position
    glEnableVertexAttribArray(nAttrib);
    glVertexAttribPointer(nAttrib, 3, GL_FLOAT, GL_FALSE, szVert, (void *) offset);
    ++nAttrib;
    offset += 3 * sizeof(float);
    // normal
    if (!normals.empty()) {
        glEnableVertexAttribArray(nAttrib);
        glVertexAttribPointer(nAttrib, 3, GL_FLOAT, GL_FALSE, szVert, (void *) offset);
        ++nAttrib;
        offset += 3 * sizeof(float);
    }
    // uv
    if (!uvs.empty()) {
        glEnableVertexAttribArray(nAttrib);
        glVertexAttribPointer(nAttrib, 2, GL_FLOAT, GL_FALSE, szVert, (void *) offset);
        ++nAttrib;
        offset += 2 * sizeof(float);
    }
    // tangent
    if (!tangents.empty()) {
        glEnableVertexAttribArray(nAttrib);
        glVertexAttribPointer(nAttrib, 3, GL_FLOAT, GL_FALSE, szVert, (void *) offset);
        ++nAttrib;
        offset += 3 * sizeof(float);
        // bitangent
        glEnableVertexAttribArray(nAttrib);
        glVertexAttribPointer(nAttrib, 3, GL_FLOAT, GL_FALSE, szVert, (void *) offset);
        ++nAttrib;
        offset += 3 * sizeof(float);
    }

    glBindVertexArray(0);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    isSetup = true;
}

void Mesh::Draw(const Shader &sh) const {
    if (!IsValid() || !sh.IsValid())
        return;
    if (!isSetup)
        SetUp();

    sh.Use();

    sh.SetInt("normal_isImg", 0);
    sh.SetInt("ao_isImg", 0);

    int nTex = 0;
    for (const Texture &t : textures) {
        std::string typeStr = t.GetTypeString();
        if (t.isImg) {
            glActiveTexture(GL_TEXTURE0 + nTex);
            glBindTexture(GL_TEXTURE_2D, t.imgID);
            sh.SetInt(typeStr + "_isImg", 1);
            sh.SetInt(typeStr + "_i", nTex);
            ++nTex;
        } else {
            sh.SetInt(typeStr + "_isImg", 0);
            sh.SetVec3(typeStr + "_c", t.color);
        }
    }

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

}
