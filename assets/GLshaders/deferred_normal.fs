#version 330 core

in vec2 uv;

out vec4 fragColor;

// G buffers
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gAmbient;
uniform sampler2D gSpecExp;

// lights
const int MAX_LIGHT = 10;
uniform int nLight;
uniform vec3 viewPos;
uniform vec3 lightPos[MAX_LIGHT];
uniform vec3 lightColor[MAX_LIGHT];

void main() {
    // get values from G buffers
    vec3 pos = texture(gPosition, uv).rgb;
    vec3 normal = texture(gNormal, uv).rgb;
    vec3 albedo_v = texture(gAlbedo, uv).rgb;
    vec3 ambient_v = texture(gAmbient, uv).rgb;
    vec4 spec_exp = texture(gSpecExp, uv);
    vec3 specular_v = spec_exp.rgb;
    float exp = spec_exp.a;

    // discard frag that with nothing on it
    if (length(normal) < 0.8)
        discard;

    vec3 result = (pos + vec3(1.0f)) / 2.0f;
    fragColor = vec4(result, 1.0f);
}