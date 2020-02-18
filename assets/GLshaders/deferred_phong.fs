#version 330 core

in vec2 uv;

out vec4 fragColor;

// G buffers
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gAmbient;
uniform sampler2D gSpecExp;
// SSAO
uniform int enableSSAO;
uniform sampler2D SSAO;

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
    float occlusion = 1.0f;
    if (enableSSAO != 0)
        occlusion = texture(SSAO, uv).r;

    // discard frag that with nothing on it
    if (length(normal) < 0.8)
        discard;

    // ambient
    vec3 ambient = ambient_v * occlusion;

    // diffuse & specular
    vec3 diffuse = vec3(0.0f);
    vec3 specular = vec3(0.0f);
    vec3 viewDir = normalize(viewPos - pos);
    for (int i = 0; i < nLight && i < MAX_LIGHT; i++) {
        // diffuse
        vec3 lightDir = normalize(lightPos[i] - pos);
        float diff = max(dot(normal, lightDir), 0.0f);
        vec3 diffuse_p = diff * lightColor[i];

        // specular
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0f), exp);
        vec3 specular_p = spec * specular_v * lightColor[i];

        // attenuation
        float dist = length(lightDir);
        float atten = 1.0f / (dist * dist);
        diffuse += diffuse_p * atten;
        specular += specular_p * atten;
    }

    vec3 result = (ambient + diffuse + specular) * albedo_v;
    result /= (result + 1.0f);
    result = pow(result, vec3(1.0f / 2.2f));
    fragColor = vec4(result, 1.0f);
}