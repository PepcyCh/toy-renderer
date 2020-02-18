#include "Window.h"
#include "InputManager.h"
#include "Scene.h"
#include "OBJLoader.h"
#include "FrameBuffer.h"
#include "MeshUtil.h"
#include "CubeFB.h"

using namespace toy_renderer;

Scene MetallicRoughnessSpheres();

int main() {
    WindowConfig config;
    config.title = "toy-renderer";
    config.width = 800;
    config.height = 600;
    Window window(config);
    if (!window.IsValid())
        return -1;

    InputManager input;
    input.BindWindow(&window);
    defaultFrameBuffer.SetGeometry(config.width, config.height);

    Camera cam(Point3f(-5, 0, 0), Point3f(0, 0, 0), Vector3f(0, 1, 0),
               Radiance(90), window.GetAspect());

    OBJLoader loader;
    // Scene scene = loader.ReadFile(scenePath + "/cube_simple.obj");
    // Scene scene = loader.ReadFile(scenePath + "/cup.obj", true);
    // Scene scene = loader.ReadFile(scenePath + "/nanosuit/nanosuit.obj", true);
    // Scene scene = loader.ReadFile(scenePath + "/cube_texture/cube_texture.obj", true);
    // scene.UpdateSkybox(Cubemap(texturePath + "/skybox/", ".jpg", true));
    Scene scene = MetallicRoughnessSpheres();
    scene.SetClearColor(0.1, 0.2, 0.3);

    Shader sh_phong(GLshaderPath + "/P3N3U2T3B3.vs",
                    GLshaderPath + "/P3N3U2T3B3_phong.fs");
    Shader sh_blinn(GLshaderPath + "/P3N3U2T3B3.vs",
                    GLshaderPath + "/P3N3U2T3B3_blinn.fs");
    Shader sh_torrance(GLshaderPath + "/P3N3U2T3B3.vs",
                       GLshaderPath + "/P3N3U2T3B3_torrance.fs");

    Shader sh_cubemap(GLshaderPath + "/cubemap.vs",
                      GLshaderPath + "/cubemap.fs");

    Shader sh_deferred(GLshaderPath + "/P3N3U2T3B3.vs",
                       GLshaderPath + "/P3N3U2T3B3_deferred.fs");
    Shader sh_screen(GLshaderPath + "/screen.vs",
                     GLshaderPath + "/deferred_phong.fs");
    Shader sh_SSAO(GLshaderPath + "/screen.vs",
                   GLshaderPath + "/deferred_SSAO.fs");

    DeferredRenderingFB deferredBuf(config.width, config.height);
    SSAOFB SSAOBuf(config.width, config.height);

    // main loop
    while (!window.ShouldClose()) {
        double deltaTime = window.GetDeltaTime();
        cam.MoveByKey(input, deltaTime);
        cam.RotateByMouse(input, deltaTime);

        // scene.Draw(cam, sh_phong, sh_cubemap);
        // scene.Draw(cam, sh_blinn, sh_cubemap);
        // scene.DrawDeferred(cam, sh_deferred, sh_screen, sh_cubemap, deferredBuf);
        // scene.DrawWithSSAO(cam, sh_deferred, sh_SSAO, sh_screen, sh_cubemap, deferredBuf, SSAOBuf);
        scene.Draw(cam, sh_torrance, sh_cubemap);

        window.Flash();
    }

    return 0;
}

Scene MetallicRoughnessSpheres() {
    Scene scene;

    const int M_COUNT = 9;
    const int R_COUNT = 9;

    const Float WIDTH = R_COUNT * 3 / 2.0;
    const Float HEIGHT = M_COUNT * 3 / 2.0;

    for (int i = 0; i <= M_COUNT; i++) {
        Float m = Float(i) / M_COUNT;
        for (int j = 0; j < R_COUNT; j++) {
            Float r = Float(j) / R_COUNT;

            Mesh mesh = MeshUtil::Sphere();
            Material mat;
            mat.emplace_back(Texture::AMBIENT, 0.2, 0.2, 0.2);
            // mat.emplace_back(Texture::ALBEDO, 0.95, 0.64, 0.54);
            mat.emplace_back(Texture::ALBEDO, 0.56, 0.57, 0.58);
            mat.emplace_back(Texture::METALLIC, m, m, m);
            mat.emplace_back(Texture::ROUGHNESS, r, r, r);
            mesh.UpdateMaterial(mat);

            Point3f pos(-10, 3 * i - HEIGHT, 3 * j - WIDTH);
            scene.AddModel(Model(mesh, Translate(pos)));
        }
    }

    scene.AddLight(Light(Point3f(10, 0, 0), Spectrum(7, 7, 7)));

    Shader sh_equirec2cubemp(GLshaderPath + "/cubemap.vs",
                             GLshaderPath + "/equirectangular2cubemap.fs");
    CubeFB cubeBuf(512);
    cubeBuf.FromEquirectangular(
            texturePath + "/HS-Gold-Room/Mt-Washington-Gold-Room_Ref.hdr",
            sh_equirec2cubemp);
    scene.UpdateSkybox(Cubemap(cubeBuf.GetColorAttachment()));
    scene.PBRSetUp();

    return scene;
}