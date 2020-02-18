#include "glad/glad.h"
#include "FrameBuffer.h"

namespace toy_renderer {

FrameBuffer::ScreenVAO FrameBuffer::screenVAO;
FrameBuffer defaultFrameBuffer;

void FrameBuffer::SetGeometry(int width, int height) {
    if (fboID == 0) {
        this->width = width;
        this->height = height;
    }
}

std::pair<int, int> FrameBuffer::GetGeometry() const {
    return std::make_pair(width, height);
}

void FrameBuffer::Use(bool retina) const {
    glBindFramebuffer(GL_FRAMEBUFFER, fboID);
    if (retina) {
        glViewport(0, 0, width * 2, height * 2);
    } else {
        glViewport(0, 0, width, height);
    }
}

void FrameBuffer::DrawScreenQuad(const Shader &sh) const {
    screenVAO.Construct();

    sh.Use();
    glBindVertexArray(screenVAO.VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

FrameBuffer::ScreenVAO::~ScreenVAO() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void FrameBuffer::ScreenVAO::Construct() {
    if (VAO != 0)
        return;

    const float screenVertices[] = {
        // pos              uv
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,

         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(screenVertices), screenVertices,
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
            (void *) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
            (void *) (3 * sizeof(float)));

    glBindVertexArray(0);
}

}