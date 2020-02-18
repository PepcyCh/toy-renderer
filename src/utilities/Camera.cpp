#include "Camera.h"

namespace toy_renderer {

Camera::Camera(const toy_renderer::Point3f &pos,
               const toy_renderer::Point3f &lookAt,
               const toy_renderer::Vector3f &up,
               Float fov, Float aspect, Float near, Float far)
               : pos(pos), fov(fov), aspect(aspect), near(near), far(far) {
    forward = Normalize(lookAt - pos);
    right = Normalize(Cross(forward, up));
    this->up = Cross(right, forward);

    view = LookAt(pos, lookAt, up);
    projection = Perspective(fov, aspect, near, far);
    projection_view = projection * view;
}

Matrix44 Camera::GetMatrix() const {
    return projection_view;
}

Matrix44 Camera::GetViewMatrix() const {
    return view;
}

Matrix44 Camera::GetProjectionMatrix() const {
    return projection;
}

Point3f Camera::GetPosition() const {
    return pos;
}

void Camera::MoveForward(Float delta) {
    pos += forward * delta;
    view = LookAt(pos, pos + forward, up);
    projection_view = projection * view;
}

void Camera::MoveUp(Float delta) {
    pos += up * delta;
    view = LookAt(pos, pos + forward, up);
    projection_view = projection * view;
}

void Camera::MoveRight(Float delta) {
    pos += right * delta;
    view = LookAt(pos, pos + forward, up);
    projection_view = projection * view;
}

void Camera::MoveByKey(const InputManager &input, double deltaTime) {
    if (input.GetKeyState(GLFW_KEY_W) == GLFW_PRESS)
        MoveForward(deltaTime);
    if (input.GetKeyState(GLFW_KEY_S) == GLFW_PRESS)
        MoveForward(-deltaTime);
    if (input.GetKeyState(GLFW_KEY_D) == GLFW_PRESS)
        MoveRight(deltaTime);
    if (input.GetKeyState(GLFW_KEY_A) == GLFW_PRESS)
        MoveRight(-deltaTime);
    if (input.GetKeyState(GLFW_KEY_Q) == GLFW_PRESS)
        MoveUp(deltaTime);
    if (input.GetKeyState(GLFW_KEY_E) == GLFW_PRESS)
        MoveUp(-deltaTime);
}

void Camera::RotateByMouse(const InputManager &input, double deltaTime) {
    auto [deltaX, deltaY] = input.GetCursorDelta();
    Matrix44 mHorizontal = Rotate(Vector3f(0, 1, 0), deltaX * deltaTime);
    forward = mHorizontal * forward;
    right = mHorizontal * right;
    up = mHorizontal * up;
    Matrix44 mVertical = Rotate(right, deltaY * deltaTime);
    forward = mVertical * forward;
    up = mVertical * up;
    view = LookAt(pos, pos + forward, up);
    projection_view = projection * view;
}

}