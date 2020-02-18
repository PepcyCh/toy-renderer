#include "glad/glad.h"
#include "SSAOFB.h"
#include <random>

namespace toy_renderer {

SSAOFB::SSAOFB(int width, int height) {
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

    // color
    unsigned int colorTex;
    glGenTextures(1, &colorTex);
    glBindTexture(GL_TEXTURE_2D, colorTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RGB,
                 GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, colorTex, 0);
    color = colorTex;

    // samples and noises
    std::uniform_real_distribution<float> random(0.0f, 1.0f);
    std::mt19937 generator;

    samples.resize(MAX_SAMPLE);
    for (int i = 0; i < MAX_SAMPLE; i++) {
        Vector3f sample(random(generator) * 2.0f - 1.0f,
                        random(generator) * 2.0f - 1.0f,
                        random(generator));
        sample = Normalize(sample);
        sample *= random(generator);
        float scale = float(i) / MAX_SAMPLE;
        scale = Lerp(0.1, 1.0, scale);
        samples[i] = sample * scale;
    }

    noises.resize(16);
    for (GLuint i = 0; i < 16; i++) {
        noises[i] = Vector3f(random(generator) * 2.0f - 1.0f,
                             random(generator) * 2.0f - 1.0f,
                             0.0f);
    }

    glGenTextures(1, &noisesTex);
    glBindTexture(GL_TEXTURE_2D, noisesTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT,
                 noises.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //  bind back to default
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int SSAOFB::GetColorAttachment() const {
    return color;
}

void SSAOFB::SetToShader(const Shader &sh) const {
    sh.Use();

    // samples
    int nSample = 0;
    for (const Vector3f &v : samples) {
        sh.SetVec3("samples[" + std::to_string(nSample) + "]", v);
        ++nSample;
    }

    // noises
    glActiveTexture(GL_TEXTURE2); // 0 - pos, 1- norm
    glBindTexture(GL_TEXTURE_2D, noisesTex);
    sh.SetInt("noises", 2);
}

void SSAOFB::Clear() {
    glDeleteFramebuffers(1, &fboID);
    glDeleteTextures(1, &color);
    glDeleteTextures(1, &noisesTex);
    fboID = 0;
}

}