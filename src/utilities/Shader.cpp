#include "glad/glad.h"
#include "defines.h"
#include "Shader.h"
#include <fstream>
#include <sstream>

namespace toy_renderer {

static unsigned int LoadShadersFromFile(const std::string &vsFile,
        const std::string &fsFile,
        const std::string &gsFile = "") {
    bool hasGS = gsFile != "";

    std::string vsCode, fsCode, gsCode;
    std::fstream vsFs, fsFs, gsFs;
    vsFs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fsFs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    gsFs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        vsFs.open(vsFile);
        fsFs.open(fsFile);
        std::stringstream vsSs, fsSs;
        vsSs << vsFs.rdbuf();
        fsSs << fsFs.rdbuf();
        vsCode = vsSs.str();
        fsCode = fsSs.str();
        vsFs.close();
        fsFs.close();
        if (hasGS) {
            std::stringstream  gsSs;
            gsFs.open(gsFile);
            gsSs << gsFs.rdbuf();
            gsCode = gsSs.str();
            gsFs.close();
        }
    } catch (const std::ifstream::failure &e) {
        LOG(e.what() << ", error code = " << e.code());
    }

    int ret;
    char logInfo[512];
    unsigned int ID = glCreateProgram();
    bool success = true;

    const char *vsStr = vsCode.c_str();
    int vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vsStr, nullptr);
    glCompileShader(vs);
    glGetShaderiv(vs, GL_COMPILE_STATUS, &ret);
    if (!ret) {
        glGetShaderInfoLog(vs, 512, nullptr, logInfo);
        LOG("CE on vertex shader, info: " << logInfo);
        success = false;
    }
    glAttachShader(ID, vs);

    const char *fsStr = fsCode.c_str();
    int fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fsStr, nullptr);
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &ret);
    if (!ret) {
        glGetShaderInfoLog(fs, 512, nullptr, logInfo);
        LOG("CE on fragment shader, info: " << logInfo);
        success = false;
    }
    glAttachShader(ID, fs);

    int gs;
    if (hasGS) {
        const char *gsStr = gsCode.c_str();
        gs = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(gs, 1, &gsStr, nullptr);
        glCompileShader(gs);
        glGetShaderiv(gs, GL_COMPILE_STATUS, &ret);
        if (!ret) {
            glGetShaderInfoLog(gs, 512, nullptr, logInfo);
            LOG("CE on geometry shader, info: " << logInfo);
            success = false;
        }
        glAttachShader(ID, gs);
    }

    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &ret);
    if (!ret) {
        glGetProgramInfoLog(ID, 512, nullptr, logInfo);
        LOG("link error on shader program (" << vsFile << ", " <<
                fsFile << ", info: " << logInfo);
    }

    glDeleteShader(vs);
    glDeleteShader(fs);
    if (hasGS) glDeleteShader(gs);

    if (!success) ID = 0;
    return ID;
}

Shader::Shader(const std::string &vsFile, const std::string &fsFile) {
    ID = LoadShadersFromFile(vsFile, fsFile);
}

Shader::Shader(const std::string &vsFile, const std::string &fsFile,
        const std::string &gsFile) {
    ID = LoadShadersFromFile(vsFile, fsFile, gsFile);
}

bool Shader::IsValid() const {
    return ID != 0;
}

void Shader::Use() const {
    glUseProgram(ID);
}

void Shader::SetInt(const std::string &name, int value) const {
    int location = glGetUniformLocation(ID, name.c_str());
    glUniform1i(location, value);
}

void Shader::SetFloat(const std::string &name, float value) const {
    int location = glGetUniformLocation(ID, name.c_str());
    glUniform1f(location, value);
}

void Shader::SetVec3(const std::string &name, const Vector3f &v) const {
    int location = glGetUniformLocation(ID, name.c_str());
    glUniform3f(location, v.x, v.y, v.z);
}

void Shader::SetVec3(const std::string &name, const Point3f &p) const {
    int location = glGetUniformLocation(ID, name.c_str());
    glUniform3f(location, p.x, p.y, p.z);
}

void Shader::SetVec3(const std::string &name, const Normal3f &n) const {
    int location = glGetUniformLocation(ID, name.c_str());
    glUniform3f(location, n.x, n.y, n.z);
}

void Shader::SetVec3(const std::string &name, const Spectrum &s) const {
    int location = glGetUniformLocation(ID, name.c_str());
    glUniform3f(location, s.r, s.g, s.b);
}

void Shader::SetMat4(const std::string &name, const Matrix44 &m) const {
    int location = glGetUniformLocation(ID, name.c_str());
#ifdef TR_FP_64bit
    float data[16] = { m[0][0], m[1][0], m[2][0], m[3][0],
                       m[0][1], m[1][1], m[2][1], m[3][1],
                       m[0][2], m[1][2], m[2][2], m[3][2],
                       m[0][3], m[1][3], m[2][3], m[3][3] };
    glUniformMatrix4fv(location, 1, GL_FALSE, data);
#else
    glUniformMatrix4fv(location, 1, GL_TRUE, &m[0][0]);
#endif
}

}
