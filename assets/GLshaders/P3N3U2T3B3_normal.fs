#version 330 core

in VS_OUT {
    vec3 pos;
    vec3 normal;
    vec2 uv;
    vec3 tangent;
    vec3 bitangent;
} vs_out;

out vec4 fragColor;

uniform int normal_isImg;
uniform sampler2D normal_i;

void main() {
    vec3 normal = vs_out.normal;
    if (normal_isImg != 0) {
        vec3 normal_t = texture(normal_i, vs_out.uv).rgb;
        normal_t = normal_t * 2.0f - 1.0f;
        normal = normalize(normal_t);
        mat3 TBN = mat3(vs_out.tangent, vs_out.bitangent, vs_out.normal);
        normal = TBN * normal;
    }

    vec3 result = (normal + 1.0f) / 2.0f;
    fragColor = vec4(result, 1.0f);
}