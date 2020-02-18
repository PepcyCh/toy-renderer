#include "glad/glad.h"
#include "Window.h"
#include "defines.h"

namespace toy_renderer {

Window::Window(const WindowConfig &config) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, config.glMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, config.glMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, config.isCore ? GLFW_OPENGL_CORE_PROFILE : GLFW_OPENGL_COMPAT_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    width = config.width;
    height = config.height;

    window = glfwCreateWindow(config.width, config.height, config.title.c_str(), nullptr, nullptr);
    if (window == nullptr) {
        LOG("Fail to create GLFW window");
        glfwTerminate();
    } else {
        glfwMakeContextCurrent(window);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            LOG("Fail to load GLAD");
            glfwTerminate();
            window = nullptr;
        } else {
            glViewport(0, 0, config.width, config.height);
            LOG("GL_VERSION: " << glGetString(GL_VERSION));
            LOG("GL_VENDOR: " << glGetString(GL_VENDOR));
            LOG("GL_RENDERER: " << glGetString(GL_RENDERER));

            lastTime = currTime = glfwGetTime();
        }
    }
}

Window::~Window() {
    glfwTerminate();
}

bool Window::IsValid() const {
    return window != nullptr;
}

bool Window::ShouldClose() const {
    return glfwWindowShouldClose(window);
}

void Window::Flash() {
    glfwSwapBuffers(window);
    glfwPollEvents();
    lastTime = currTime;
    currTime = glfwGetTime();
}

double Window::GetAspect() const {
    return double(width) / height;
}

double Window::GetDeltaTime() const {
    return currTime - lastTime;
}

}