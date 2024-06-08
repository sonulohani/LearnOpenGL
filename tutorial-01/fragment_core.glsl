#version 440

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;
in vec3 vs_normal;

out vec4 fs_color;

uniform sampler2D texture0;
uniform sampler2D texture1;

uniform vec3 lightPos0;

void main()
{
    fs_color = vec4(vs_color, 1.f);
    // fs_color *= texture(texture0, vs_texcoord) * texture(texture1, vs_texcoord);

    // Ambient light
    vec3 ambientLight = vec3(0.1f, 0.1f, 0.1f);

    // Diffuse light
    vec3 lightDir = normalize(vs_position - lightPos0);
    float diffuseIntensity = max(dot(vs_normal, lightDir), 0);
    vec4 diffuseColor = vec4(1.f, 1.f, 1.f, 1.f) * diffuseIntensity;

    fs_color *= texture(texture0, vs_texcoord) * (vec4(ambientLight, 1.f) + diffuseColor);
}