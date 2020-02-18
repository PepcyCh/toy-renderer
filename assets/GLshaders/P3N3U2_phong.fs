#version 330 core

in VS_OUT {
    vec3 pos;
    vec3 normal;
    vec2 uv;
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
// exponent
uniform vec3 exponent_c;

void main() {
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
        float diff = max(dot(vs_out.normal, lightDir), 0.0f);
        diffuse += diff * lightColor[i];

        // specular
        vec3 reflectDir = reflect(-lightDir, vs_out.normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0f), exp);
        specular += spec * specular_v * lightColor[i];
    }

    vec3 result = (ambient + diffuse + specular) * albedo_v;
    result /= (result + 1.0f);
    result = pow(result, vec3(1.0f / 2.2f));
    fragColor = vec4(result, 1.0f);
}