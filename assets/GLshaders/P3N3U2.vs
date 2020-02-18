#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

out VS_OUT {
    vec3 pos;
    vec3 normal;
    vec2 uv;
} vs_out;

uniform mat4 projection_view;
uniform mat4 model;
uniform mat4 model_TI;

void main() {
    gl_Position = projection_view * model * vec4(pos, 1.0f);
    vs_out.pos = vec3(model * vec4(pos, 1.0f));
    vs_out.normal = normalize(mat3(model_TI) * normal);
    vs_out.uv = uv;
}