#include "glad/glad.h"
#include "MeshUtil.h"

namespace toy_renderer {

Mesh MeshUtil::Cube() {
    const std::vector<Point3f> pos({
        Point3f(-1.0f,  1.0f, -1.0f),
        Point3f(-1.0f, -1.0f, -1.0f),
        Point3f( 1.0f, -1.0f, -1.0f),
        Point3f( 1.0f,  1.0f, -1.0f),

        Point3f(-1.0f, -1.0f,  1.0f),
        Point3f(-1.0f, -1.0f, -1.0f),
        Point3f(-1.0f,  1.0f, -1.0f),
        Point3f(-1.0f,  1.0f,  1.0f),

        Point3f( 1.0f, -1.0f, -1.0f),
        Point3f( 1.0f, -1.0f,  1.0f),
        Point3f( 1.0f,  1.0f,  1.0f),
        Point3f( 1.0f,  1.0f, -1.0f),

        Point3f(-1.0f, -1.0f,  1.0f),
        Point3f(-1.0f,  1.0f,  1.0f),
        Point3f( 1.0f,  1.0f,  1.0f),
        Point3f( 1.0f, -1.0f,  1.0f),

        Point3f(-1.0f,  1.0f, -1.0f),
        Point3f( 1.0f,  1.0f, -1.0f),
        Point3f( 1.0f,  1.0f,  1.0f),
        Point3f(-1.0f,  1.0f,  1.0f),

        Point3f(-1.0f, -1.0f, -1.0f),
        Point3f(-1.0f, -1.0f,  1.0f),
        Point3f( 1.0f, -1.0f,  1.0f),
        Point3f( 1.0f, -1.0f, -1.0f)
    });
    const std::vector<Normal3f> normals({
        Normal3f( 0.0f,  0.0f, -1.0f),
        Normal3f( 0.0f,  0.0f, -1.0f),
        Normal3f( 0.0f,  0.0f, -1.0f),
        Normal3f( 0.0f,  0.0f, -1.0f),

        Normal3f(-1.0f,  0.0f,  0.0f),
        Normal3f(-1.0f,  0.0f,  0.0f),
        Normal3f(-1.0f,  0.0f,  0.0f),
        Normal3f(-1.0f,  0.0f,  0.0f),

        Normal3f( 1.0f,  0.0f,  0.0f),
        Normal3f( 1.0f,  0.0f,  0.0f),
        Normal3f( 1.0f,  0.0f,  0.0f),
        Normal3f( 1.0f,  0.0f,  0.0f),

        Normal3f( 0.0f,  0.0f,  1.0f),
        Normal3f( 0.0f,  0.0f,  1.0f),
        Normal3f( 0.0f,  0.0f,  1.0f),
        Normal3f( 0.0f,  0.0f,  1.0f),

        Normal3f( 0.0f,  1.0f,  0.0f),
        Normal3f( 0.0f,  1.0f,  0.0f),
        Normal3f( 0.0f,  1.0f,  0.0f),
        Normal3f( 0.0f,  1.0f,  0.0f),

        Normal3f( 0.0f, -1.0f,  0.0f),
        Normal3f( 0.0f, -1.0f,  0.0f),
        Normal3f( 0.0f, -1.0f,  0.0f),
        Normal3f( 0.0f, -1.0f,  0.0f)
    });
    const std::vector<Point2f> uvs({
        Point2f(0.0f, 1.0f), Point2f(0.0f, 0.0f),
        Point2f(1.0f, 0.0f), Point2f(1.0f, 1.0f),

        Point2f(0.0f, 1.0f), Point2f(0.0f, 0.0f),
        Point2f(1.0f, 0.0f), Point2f(1.0f, 1.0f),

        Point2f(0.0f, 0.0f), Point2f(0.0f, 1.0f),
        Point2f(1.0f, 1.0f), Point2f(1.0f, 0.0f),

        Point2f(0.0f, 0.0f), Point2f(0.0f, 1.0f),
        Point2f(1.0f, 1.0f), Point2f(1.0f, 0.0f),

        Point2f(0.0f, 0.0f), Point2f(0.0f, 1.0f),
        Point2f(1.0f, 1.0f), Point2f(1.0f, 0.0f),

        Point2f(0.0f, 0.0f), Point2f(0.0f, 1.0f),
        Point2f(1.0f, 1.0f), Point2f(1.0f, 0.0f)
    });
    const std::vector<unsigned int> indices({
        0, 2, 1, 0, 3, 2,
        4, 6, 5, 4, 7, 6,
        8, 10, 9, 8, 11, 10,
        12, 14, 13, 12, 15, 14,
        16, 18, 17, 16, 19, 18,
        20, 22, 21, 20, 23, 22
    });

    Mesh mesh;
    mesh.UpdatePositions(pos);
    mesh.UpdateNormals(normals);
    mesh.UpdateUVs(uvs);
    mesh.UpdateIndices(indices);
    mesh.CalcTangents();

    return mesh;
}

Mesh MeshUtil::Sphere() {
    std::vector<Point3f> pos;
    std::vector<Normal3f> normals;
    std::vector<Point2f> uvs;
    std::vector<unsigned int> indices;

    const int Y_COUNT = 24;
    const int X_COUNT = 2 * Y_COUNT;

    for (int j = 0; j <= Y_COUNT; j++) {
        Float y = Float(j) / Y_COUNT;
        for (int i = 0; i < X_COUNT; i++) {
            Float x = Float(i) / X_COUNT;

            Float xPos = std::cos(x * 2.0 * PI) * std::sin(y * PI);
            Float yPos = std::cos(y * PI);
            Float zPos = std::sin(x * 2.0 * PI) * std::sin(y * PI);

            pos.emplace_back(xPos, yPos, zPos);
            uvs.emplace_back(x, y);
            normals.emplace_back(xPos, yPos, zPos);
        }
        int N = pos.size() - X_COUNT;
        pos.emplace_back(pos[N]);
        uvs.emplace_back(1, y);
        normals.emplace_back(normals[N]);
    }
    for (int j = 0; j < Y_COUNT; j++) {
        for (int i = 0; i <= X_COUNT; i++) {
            indices.push_back( j      * (X_COUNT + 1) + i);
            indices.push_back((j + 1) * (X_COUNT + 1) + i + 1);
            indices.push_back((j + 1) * (X_COUNT + 1) + i);
            indices.push_back( j      * (X_COUNT + 1) + i);
            indices.push_back( j      * (X_COUNT + 1) + i + 1);
            indices.push_back((j + 1) * (X_COUNT + 1) + i + 1);
        }
    }

    Mesh mesh;
    mesh.UpdatePositions(pos);
    mesh.UpdateNormals(normals);
    mesh.UpdateUVs(uvs);
    mesh.UpdateIndices(indices);
    mesh.CalcTangents();

    return mesh;
}

unsigned int MeshUtil::CubeVAO() {
    static unsigned int VAO = 0;

    if (VAO != 0)
        return VAO;

    const float vertices[] = {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    unsigned int VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

    glBindVertexArray(0);
    glDeleteBuffers(1, &VBO);

    return VAO;
}

}