#pragma once

#include "geometry.h"
#include "InputManager.h"

namespace toy_renderer {

class Camera {
  public:
    Camera(const Point3f &pos, const Point3f &lookAt, const Vector3f &up,
           Float fov, Float aspect, Float near = 0.1, Float far = 100);

    Matrix44 GetMatrix() const;
    Matrix44 GetViewMatrix() const;
    Matrix44 GetProjectionMatrix() const;
    Point3f GetPosition() const;

    void MoveForward(Float delta);
    void MoveUp(Float delta);
    void MoveRight(Float delta);
    void MoveByKey(const InputManager &input, double deltaTime);

    void RotateByMouse(const InputManager &input, double deltaTime);

  private:
    Point3f pos;
    Vector3f forward, up, right;
    Matrix44 view, projection, projection_view;
    Float fov, aspect;
    Float near, far;
};

}