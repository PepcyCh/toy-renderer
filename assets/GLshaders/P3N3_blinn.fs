#version 330 core

in VS_OUT {
    vec3 pos;
    vec3 normal;
} vs_out;

out vec4 fragColor;

const int MAX_LIGHT = 10;
uniform int nLight;
uniform vec3 viewPos;
uniform vec3 lightPos[MAX_LIGHT];
uniform vec3 lightColor[MAX_LIGHT];
uniform vec3 ambient_c;
uniform vec3 albedo_c;
uniform vec3 specular_c;
uniform vec3 exponent_c;

void main() {
    // ambient
    vec3 ambient = ambient_c;

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
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(normal, halfwayDir), 0.0f), exp);
        specular += spec * specular_c * lightColor[i];
    }

    vec3 result = (ambient + diffuse + specular) * albedo_c;
    result /= (result + 1.0f);
    result = pow(result, vec3(1.0f / 2.2f));
    fragColor = vec4(result, 1.0f);
}