#include "glad/glad.h"
#include "Scene.h"
#include "TextureManager.h"
#include "FrameBuffer.h"
#include "CubeFB.h"
#include "MeshUtil.h"

namespace toy_renderer {

void Scene::AddLight(const Light &l) {
    lights.push_back(l);
}

void Scene::AddModel(const Model &m) {
    models.push_back(m);
}

void Scene::UpdateSkybox(const Cubemap &cm) {
    skybox = cm;
}

void Scene::UpdateEnvironmentMap(const Cubemap &cm) {
    irradMap = cm.GetID();
}
void Scene::UpdateEnvironmentMap(int cm) {
    irradMap = cm;
}

void Scene::UpdateReflectionMap(const Cubemap &cm) {
    irradMap = cm.GetID();
}
void Scene::UpdateReflectionMap(int cm) {
    irradMap = cm;
}

void Scene::SetClearColor(const Spectrum &color) {
    clearColor = color;
}
void Scene::SetClearColor(Float r, Float g, Float b) {
    clearColor = Spectrum(r, g, b);
}

void Scene::PBRSetUp() {
    if (skybox.GetID() == 0)
        return;

    unsigned int cubeVAO = MeshUtil::CubeVAO();

    // calc irradMap
    Shader sh_irradMap(GLshaderPath + "/cubemap.vs",
                       GLshaderPath + "/pbr_irradmap.fs");
    CubeFB irradMapBuf(512, true);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.GetID());
    sh_irradMap.Use();
    sh_irradMap.SetInt("cubemap", 0);
    irradMapBuf.DrawToBuffer(sh_irradMap);
    irradMap = irradMapBuf.GetColorAttachment();

    // pre-filter mipmap
    CubeFB prefilterBuf(512, true);
    reflMap = prefilterBuf.GetColorAttachment();
    glBindTexture(GL_TEXTURE_CUBE_MAP, reflMap);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    Shader sh_prefilter(GLshaderPath + "/cubemap.vs",
                        GLshaderPath + "/pbr_prefilter.fs");
    sh_prefilter.Use();
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.GetID());
    sh_prefilter.SetInt("cubemap", 0);
    for (int mip = 0; mip < 5; mip++) {
        float roughness = mip / 4.0f;
        sh_prefilter.SetFloat("roughness", roughness);
        prefilterBuf.DrawToBuffer(sh_prefilter, mip);
    }

    defaultFrameBuffer.Use();
}

void Scene::Clear() {
    models.clear();
    lights.clear();
    clearColor = Spectrum();
}

void Scene::Draw(const Camera &cam, const Shader &modelSh,
        const Shader &cubemapSh) const {
    // draw without skybox
    Draw(cam, modelSh);

    // draw skybox
    skybox.Draw(cam, cubemapSh);
}

void Scene::Draw(const Camera &cam, const Shader &sh) const {
    // clear
    glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // set matrices & cubemap
    sh.Use();
    sh.SetMat4("projection_view", cam.GetMatrix());
    sh.SetVec3("viewPos", cam.GetPosition());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, reflMap);
    sh.SetInt("reflMap", 1);
    sh.SetInt("hasReflMap", reflMap != 0);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradMap);
    sh.SetInt("irradMap", 2);
    sh.SetInt("hasIrradMap", irradMap != 0);
    glActiveTexture(GL_TEXTURE31);
    glBindTexture(GL_TEXTURE_2D, gTextureManager
            .LoadTextureFromFile(texturePath + "/BRDFLUT.png"));
    sh.SetInt("BRDFLUT", 31);

    // set lights
    int nLight = 0;
    for (const Light &l : lights) {
        l.SetToShader(sh, nLight);
        ++nLight;
    }
    sh.SetInt("nLight", nLight);

    // draw models and lights
    for (const Model &m : models) {
        m.Draw(sh);
    }
}

void Scene::DrawDeferred(const Camera &cam, const Shader &shBuf,
        const Shader &shLight, const Shader &shCube,
        const DeferredRenderingFB &buf) const {
    // draw without skybox
    DrawDeferred(cam, shBuf, shLight, buf);

    // draw skybox
    skybox.Draw(cam, shCube);
}

void Scene::DrawDeferred(const Camera &cam, const Shader &shBuf,
        const Shader &shLight, const DeferredRenderingFB &buf) const {
    // clear
    buf.Use();
    glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // draw to G buffers
    shBuf.Use();
    shBuf.SetMat4("projection_view", cam.GetMatrix());

    for (const Model &m : models) {
        m.Draw(shBuf);
    }

    // draw lights
    defaultFrameBuffer.Use(isRetina);
    glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_CULL_FACE);

    shLight.Use();
    int nLight = 0;
    for (const Light &l : lights) {
        l.SetToShader(shLight, nLight);
        ++nLight;
    }
    shLight.SetInt("nLight", nLight);
    shLight.SetVec3("viewPos", cam.GetPosition());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, reflMap);
    shLight.SetInt("reflMap", 1);
    shLight.SetInt("hasReflMap", reflMap != 0);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradMap);
    shLight.SetInt("irradMap", 2);
    shLight.SetInt("hasIrradMap", irradMap != 0);

    shLight.SetInt("enableSSAO", 0);
    buf.SetToShader(shLight);
    buf.DrawScreenQuad(shLight);

    // bind back to default
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Scene::DrawWithSSAO(const Camera &cam, const Shader &shGBuf,
        const Shader &shSSAO, const Shader &shLight, const Shader &shCube,
        const DeferredRenderingFB &GBuf, const SSAOFB &SSAOBuf) const {
    // draw without skybox
    DrawWithSSAO(cam, shGBuf, shSSAO, shLight, GBuf, SSAOBuf);

    // draw skybox
    skybox.Draw(cam, shCube);
}

void Scene::DrawWithSSAO(const Camera &cam, const Shader &shGBuf,
        const Shader &shSSAO, const Shader &shLight,
        const DeferredRenderingFB &GBuf, const SSAOFB &SSAOBuf) const {
    // draw to G buffers
    GBuf.Use();
    glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    shGBuf.Use();
    shGBuf.SetMat4("projection_view", cam.GetMatrix());
    for (const Model &m : models) {
        m.Draw(shGBuf);
    }

    // draw SSAO
    SSAOBuf.Use();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    GBuf.SetToShader(shSSAO);
    SSAOBuf.SetToShader(shSSAO);
    shSSAO.SetMat4("projection", cam.GetProjectionMatrix());
    shSSAO.SetMat4("view", cam.GetViewMatrix());
    auto [width, height] = SSAOBuf.GetGeometry();
    shSSAO.SetInt("screenWidth", width);
    shSSAO.SetInt("screenHeight", height);
    SSAOBuf.DrawScreenQuad(shSSAO);

    // draw lights
    defaultFrameBuffer.Use(isRetina);
    glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_CULL_FACE);

    shLight.Use();
    int nLight = 0;
    for (const Light &l : lights) {
        l.SetToShader(shLight, nLight);
        ++nLight;
    }
    shLight.SetInt("nLight", nLight);
    shLight.SetVec3("viewPos", cam.GetPosition());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, reflMap);
    shLight.SetInt("reflMap", 1);
    shLight.SetInt("hasReflMap", reflMap != 0);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradMap);
    shLight.SetInt("irradMap", 2);
    shLight.SetInt("hasIrradMap", irradMap != 0);

    unsigned int SSAO = SSAOBuf.GetColorAttachment();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, SSAO);
    shLight.SetInt("SSAO", 0);
    shLight.SetInt("enableSSAO", 1);
    GBuf.SetToShader(shLight, 1);
    GBuf.DrawScreenQuad(shLight);

    // bind back to default
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

}