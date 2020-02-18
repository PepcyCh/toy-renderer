#pragma once

#include "Window.h"

namespace toy_renderer {

class InputManager {
  public:
    void BindWindow(const Window *p);
    bool IsValid() const;
    int GetKeyState(int key) const;
    int GetMouseState(int btn) const;
    std::pair<double, double> GetCursorPosition() const;
    std::pair<double, double> GetCursorDelta() const;

  private:
    const Window *pWindow = nullptr;
    mutable double lastX, lastY;
};

}