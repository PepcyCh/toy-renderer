#pragma once

#include <string>
#include "glfw3.h"

namespace toy_renderer {

// glfw window config
struct WindowConfig {
    // window size
    size_t width = 800;
    size_t height = 600;
    // window title
    std::string title;
    // OpenGL version
    int glMajor = 3;
    int glMinor = 3;
    bool isCore = true;
};

class Window {
  public:
    explicit Window(const WindowConfig &config);
    ~Window();

    bool IsValid() const;
    bool ShouldClose() const;
    void Flash();

    double GetAspect() const;
    double GetDeltaTime() const;

  private:
    GLFWwindow *window;
    size_t width;
    size_t height;
    double lastTime, currTime;

    friend class InputManager;
};

}