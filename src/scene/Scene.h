#pragma once

#include "Camera.h"
#include "Spectrum.h"
#include "Cubemap.h"
#include "Light.h"
#include "Model.h"
#include "DeferredRenderingFB.h"
#include "SSAOFB.h"

namespace toy_renderer {

class Scene {
  public:
    Scene() : models(), lights() {}
    ~Scene() { Clear(); }

    void AddModel(const Model &m);
    void AddLight(const Light &l);
    void UpdateSkybox(const Cubemap &cm);
    void UpdateEnvironmentMap(const Cubemap &cm);
    void UpdateEnvironmentMap(int cm);
    void UpdateReflectionMap(const Cubemap &cm);
    void UpdateReflectionMap(int cm);
    void SetClearColor(const Spectrum &color);
    void SetClearColor(Float r, Float g, Float b);

    void PBRSetUp();

    void Clear();

    // draw (normal)
    void Draw(const Camera &cam, const Shader &modelSh,
            const Shader &cubemapSh) const;
    void Draw(const Camera &cam, const Shader &sh) const;
    // deferred rendering
    void DrawDeferred(const Camera &cam, const Shader &shBuf,
                      const Shader &shLight, const Shader &shCube,
                      const DeferredRenderingFB &buf) const;
    void DrawDeferred(const Camera &cam, const Shader &shBuf,
                      const Shader &shLight,
                      const DeferredRenderingFB &buf) const;
    // draw with SSAO
    void DrawWithSSAO(const Camera &cam, const Shader &shGBuf,
                      const Shader &shSSAO, const Shader &shLight,
                      const Shader &shCube, const DeferredRenderingFB &GBuf,
                      const SSAOFB &SSAOBuf) const;
    void DrawWithSSAO(const Camera &cam, const Shader &shGBuf,
                      const Shader &shSSAO, const Shader &shLight,
                      const DeferredRenderingFB &GBuf,
                      const SSAOFB &SSAOBuf) const;

  private:
    std::vector<Model> models;
    std::vector<Light> lights;
    Cubemap skybox;
    unsigned int irradMap, reflMap;
    Spectrum clearColor;
};

}