#version 330 core

layout (location = 0) in vec3 pos;

out vec3 direction;

uniform mat4 projection_view;

void main() {
    direction = pos;
    vec4 temp = projection_view * vec4(pos, 1.0);
    gl_Position = temp.xyww;
}