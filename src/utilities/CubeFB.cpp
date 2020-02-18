#include "glad/glad.h"
#include "CubeFB.h"
#include "TextureManager.h"
#include "MeshUtil.h"

namespace toy_renderer {

static std::vector<Camera> GetCameras(const Point3f &center) {
    return {
        Camera(center, center + Point3f( 1.0,  0.0,  0.0),
               Vector3f( 0.0, -1.0,  0.0), Radiance(90), 1),
        Camera(center, center + Point3f(-1.0,  0.0,  0.0),
               Vector3f( 0.0, -1.0,  0.0), Radiance(90), 1),
        Camera(center, center + Point3f( 0.0,  1.0,  0.0),
               Vector3f( 0.0,  0.0,  1.0), Radiance(90), 1),
        Camera(center, center + Point3f( 0.0, -1.0,  0.0),
               Vector3f( 0.0,  0.0, -1.0), Radiance(90), 1),
        Camera(center, center + Point3f( 0.0,  0.0,  1.0),
               Vector3f( 0.0, -1.0,  0.0), Radiance(90), 1),
        Camera(center, center + Point3f( 0.0,  0.0, -1.0),
               Vector3f( 0.0, -1.0,  0.0), Radiance(90), 1)
    };
}

CubeFB::CubeFB(int width, bool isHDR) {
    // clear old bufer
    if (fboID != 0) {
        Clear();
    }

    this->width = width;
    this->height = width;

    glGenFramebuffers(1, &fboID);
    glBindFramebuffer(GL_FRAMEBUFFER, fboID);

    unsigned int cubeTex;
    glGenTextures(1, &cubeTex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTex);
    color = cubeTex;
    for (int i = 0; i < 6; i++) {
        GLenum format = isHDR ? GL_RGB16F : GL_RGB;
        GLenum type = isHDR ? GL_FLOAT : GL_UNSIGNED_BYTE;
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format,
                     width, width, 0, GL_RGB, type, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glGenRenderbuffers(1, &depth);
    glBindRenderbuffer(GL_RENDERBUFFER, depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, width);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, depth);

    //  bind back to default
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void CubeFB::DrawToBuffer(const Scene &scene, const Shader &sh,
        const Point3f &center) const {
    Use();

    auto cams = GetCameras(center);
    for (int i = 0; i < 6; i++) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, color, 0);
        scene.Draw(cams[i], sh);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CubeFB::DrawToBuffer(const Scene &scene, const Shader &sh,
        const Shader &shCube, const Point3f &center) const {
    Use();

    auto cams = GetCameras(center);
    for (int i = 0; i < 6; i++) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, color, 0);
        scene.Draw(cams[i], sh, shCube);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CubeFB::FromEquirectangular(const std::string &path,
        const Shader &sh) const {
    unsigned int texID = gTextureManager.LoadHDRFromFile(path);
    if (texID == 0)
        return;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);
    sh.Use();
    sh.SetInt("equirectangular", 0);

    DrawToBuffer(sh);

    glBindTexture(GL_TEXTURE_CUBE_MAP, color);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void CubeFB::DrawToBuffer(const Shader &sh) const {
    Use();
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    Matrix44 projection = Perspective(Radiance(90), 1, 0.1, 100);
    Matrix44 view[] = {
        LookAt(Point3f(), Point3f( 1.0,  0.0,  0.0), Vector3f(0.0, -1.0,  0.0)),
        LookAt(Point3f(), Point3f(-1.0,  0.0,  0.0), Vector3f(0.0, -1.0,  0.0)),
        LookAt(Point3f(), Point3f( 0.0,  1.0,  0.0), Vector3f(0.0,  0.0,  1.0)),
        LookAt(Point3f(), Point3f( 0.0, -1.0,  0.0), Vector3f(0.0,  0.0, -1.0)),
        LookAt(Point3f(), Point3f( 0.0,  0.0,  1.0), Vector3f(0.0, -1.0,  0.0)),
        LookAt(Point3f(), Point3f( 0.0,  0.0, -1.0), Vector3f(0.0, -1.0,  0.0))
    };

    unsigned int cubeVAO = MeshUtil::CubeVAO();
    glBindVertexArray(cubeVAO);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    sh.Use();

    for (int i = 0; i < 6; i++) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, color, 0);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        sh.SetMat4("projection_view", projection * view[i]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CubeFB::DrawToBuffer(const Shader &sh, int mip) const {
    glBindFramebuffer(GL_FRAMEBUFFER, fboID);
    int vWidth = width * pow(0.5, mip);
    int vHeight = height * pow(0.5, mip);
    glViewport(0, 0, vWidth, vHeight);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    glBindRenderbuffer(GL_RENDERBUFFER, depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, vWidth, vHeight);

    Matrix44 projection = Perspective(Radiance(90), 1, 0.1, 100);
    Matrix44 view[] = {
            LookAt(Point3f(), Point3f( 1.0,  0.0,  0.0), Vector3f(0.0, -1.0,  0.0)),
            LookAt(Point3f(), Point3f(-1.0,  0.0,  0.0), Vector3f(0.0, -1.0,  0.0)),
            LookAt(Point3f(), Point3f( 0.0,  1.0,  0.0), Vector3f(0.0,  0.0,  1.0)),
            LookAt(Point3f(), Point3f( 0.0, -1.0,  0.0), Vector3f(0.0,  0.0, -1.0)),
            LookAt(Point3f(), Point3f( 0.0,  0.0,  1.0), Vector3f(0.0, -1.0,  0.0)),
            LookAt(Point3f(), Point3f( 0.0,  0.0, -1.0), Vector3f(0.0, -1.0,  0.0))
    };

    unsigned int cubeVAO = MeshUtil::CubeVAO();
    glBindVertexArray(cubeVAO);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    sh.Use();

    for (int i = 0; i < 6; i++) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, color, mip);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        sh.SetMat4("projection_view", projection * view[i]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int CubeFB::GetColorAttachment() const {
    return color;
}

void CubeFB::Clear() {
    glDeleteFramebuffers(1, &fboID);
    glDeleteTextures(1, &color);
    glDeleteRenderbuffers(1, &depth);
}

}