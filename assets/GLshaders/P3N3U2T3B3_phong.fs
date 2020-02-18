#version 330 core

in VS_OUT {
    vec3 pos;
    vec3 normal;
    vec2 uv;
    vec3 tangent;
    vec3 bitangent;
} vs_out;

out vec4 fragColor;

// lights
const int MAX_LIGHT = 10;
uniform int nLight;
uniform vec3 viewPos;
uniform vec3 lightPos[MAX_LIGHT];
uniform vec3 lightColor[MAX_LIGHT];
// ambient
uniform int ambient_isImg;
uniform vec3 ambient_c;
uniform sampler2D ambient_i;
// albedo (diffuse)
uniform int albedo_isImg;
uniform vec3 albedo_c;
uniform sampler2D albedo_i;
// specular
uniform int specular_isImg;
uniform vec3 specular_c;
uniform sampler2D specular_i;
// normal
uniform int normal_isImg;
uniform sampler2D normal_i;
// exponent
uniform vec3 exponent_c;

void main() {
    // calc normal
    vec3 normal = vs_out.normal;
    if (normal_isImg != 0) {
        vec3 normal_t = texture(normal_i, vs_out.uv).rgb;
        normal_t = normal_t * 2.0f - 1.0f;
        normal = normalize(normal_t);
        mat3 TBN = mat3(vs_out.tangent, vs_out.bitangent, vs_out.normal);
        normal = normalize(TBN * normal);
    }

    // get values according to XXX_isImg
    vec3 albedo_v = albedo_c;
    if (albedo_isImg != 0) albedo_v = texture(albedo_i, vs_out.uv).rgb;
    vec3 specular_v = specular_c;
    if (specular_isImg != 0) specular_v = texture(specular_i, vs_out.uv).rgb;

    // ambient
    vec3 ambient = ambient_c;
    if (ambient_isImg != 0) ambient = texture(ambient_i, vs_out.uv).rgb;

    // diffuse & specular
    vec3 diffuse = vec3(0.0f);
    vec3 specular = vec3(0.0f);
    vec3 viewDir = normalize(viewPos - vs_out.pos);
    float exp = exponent_c.x;
    if (exp < 0.01) exp = 32;
    for (int i = 0; i < nLight && i < MAX_LIGHT; i++) {
        // diffuse
        vec3 lightDir = normalize(lightPos[i] - vs_out.pos);
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