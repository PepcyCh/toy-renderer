#include "glad/glad.h"
#include "DeferredRenderingFB.h"

namespace toy_renderer {

DeferredRenderingFB::DeferredRenderingFB(int width, int height) {
    // clear old bufer
    if (fboID != 0) {
        Clear();
    }

    if (isRetina) {
        width *= 2;
        height *= 2;
    }
    this->width = width;
    this->height = height;

    glGenFramebuffers(1, &fboID);
    glBindFramebuffer(GL_FRAMEBUFFER, fboID);

    // position
    unsigned int gPosition;
    glGenTextures(1, &gPosition);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA,
                 GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, gPosition, 0);
    colors[0] = gPosition;

    // normal
    unsigned int gNormal;
    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB,
                 GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
                           GL_TEXTURE_2D, gNormal, 0);
    colors[1] = gNormal;

    // albedo
    unsigned int gAlbedo;
    glGenTextures(1, &gAlbedo);
    glBindTexture(GL_TEXTURE_2D, gAlbedo);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2,
                           GL_TEXTURE_2D, gAlbedo, 0);
    colors[2] = gAlbedo;

    // ambient
    unsigned int gAmbient;
    glGenTextures(1, &gAmbient);
    glBindTexture(GL_TEXTURE_2D, gAmbient);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3,
                           GL_TEXTURE_2D, gAmbient, 0);
    colors[3] = gAmbient;

    // specular - exponent
    unsigned int gSpecExp;
    glGenTextures(1, &gSpecExp);
    glBindTexture(GL_TEXTURE_2D, gSpecExp);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4,
                           GL_TEXTURE_2D, gSpecExp, 0);
    colors[4] = gSpecExp;

    glBindTexture(GL_TEXTURE_2D, 0);
    GLuint attachments[] = {
            GL_COLOR_ATTACHMENT0,
            GL_COLOR_ATTACHMENT1,
            GL_COLOR_ATTACHMENT2,
            GL_COLOR_ATTACHMENT3,
            GL_COLOR_ATTACHMENT4
    };
    glDrawBuffers(sizeof(attachments) / sizeof(GLuint), attachments);

    // depth
    unsigned int depthBuf;
    glGenRenderbuffers(1, &depthBuf);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, depthBuf);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    depth = depthBuf;

    //  bind back to default
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DeferredRenderingFB::SetToShader(const Shader &sh, int offset) const {
    sh.Use();

    const char *names[] = {
        "gPosition", "gNormal", "gAlbedo",
        "gAmbient", "gSpecExp"
    };

    int nTex = 0;
    for (unsigned int i : colors) {
        glActiveTexture(GL_TEXTURE0 + nTex + offset);
        glBindTexture(GL_TEXTURE_2D, colors[nTex]);
        sh.SetInt(names[nTex], nTex + offset);
        ++nTex;
    }
}

void DeferredRenderingFB::Clear() {
    glDeleteFramebuffers(1, &fboID);
    glDeleteTextures(sizeof(colors) / sizeof(unsigned int), colors);
    glDeleteRenderbuffers(1, &depth);
    fboID = 0;
}

}