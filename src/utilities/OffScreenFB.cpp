#include "glad/glad.h"
#include "OffScreenFB.h"

namespace toy_renderer {

OffScreenFB::OffScreenFB(int width, int height) {
    // clear old bufer
    if (fboID != 0) {
        Clear();
    }

    this->width = width;
    this->height = height;

    glGenFramebuffers(1, &fboID);
    glBindFramebuffer(GL_FRAMEBUFFER, fboID);

    // color
    unsigned int colorTex;
    glGenTextures(1, &colorTex);
    glBindTexture(GL_TEXTURE_2D, colorTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, colorTex, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    color = colorTex;

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

unsigned int OffScreenFB::GetColorAttachment() const {
    return color;
}

void OffScreenFB::Clear() {
    glDeleteFramebuffers(1, &fboID);
    glDeleteTextures(1, &color);
    glDeleteRenderbuffers(1, &depth);
    fboID = 0;
}

}