#include "glad/glad.h"
#include "CubeDepthFB.h"

namespace toy_renderer {

CubeDepthFB::CubeDepthFB(int width) {
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
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
                     width, width, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    for (int i = 0; i < 6; i++) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_COMPONENT,
                               GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubeTex, 0);
    }

    //  bind back to default
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

unsigned int CubeDepthFB::GetColorAttachment() const {
    return color;
}

void CubeDepthFB::Clear() {
    glDeleteFramebuffers(1, &fboID);
    glDeleteTextures(1, &color);
    fboID = 0;
}

}