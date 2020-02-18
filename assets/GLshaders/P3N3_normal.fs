#version 330 core

in VS_OUT {
    vec3 pos;
    vec3 normal;
} vs_out;

out vec4 fragColor;

void main() {
    vec3 result = (vs_out.normal + 1.0f) / 2.0f;
    fragColor = vec4(result, 1.0f);
}