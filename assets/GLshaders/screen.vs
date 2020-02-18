#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv_i;

out vec2 uv;

void main() {
    gl_Position = vec4(pos, 1.0f);
    uv = uv_i;
}