#version 330 core

in vec3 direction;

out vec4 fragColor;

uniform samplerCube cubemap;

void main() {
    fragColor = texture(cubemap, direction);
    fragColor = pow(fragColor, vec4(1.0f / 2.2f));
}