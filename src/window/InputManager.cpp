#include "InputManager.h"

namespace toy_renderer {

void InputManager::BindWindow(const Window *p) {
    pWindow = p;
}

bool InputManager::IsValid() const {
    return pWindow != nullptr;
}

int InputManager::GetKeyState(int key) const {
    return IsValid() ? glfwGetKey(pWindow->window, key) : -1;
}

int InputManager::GetMouseState(int btn) const {
    return IsValid() ? glfwGetMouseButton(pWindow->window, btn) : -1;
}

std::pair<double, double> InputManager::GetCursorPosition() const {
    if (!IsValid())
        return std::make_pair(0.0 / 0.0, 0.0 / 0.0);
    double x, y;
    glfwGetCursorPos(pWindow->window, &x, &y);
    lastX = x;
    lastY = y;
    return std::make_pair(x, y);
}

std::pair<double, double> InputManager::GetCursorDelta() const {
    if (!IsValid())
        return std::make_pair(0.0 / 0.0, 0.0 / 0.0);
    double x, y;
    glfwGetCursorPos(pWindow->window, &x, &y);
    double deltaX = x - lastX, delatY = y - lastY;
    lastX = x;
    lastY = y;
    return std::make_pair(deltaX, delatY);
}

}