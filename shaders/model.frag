#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    // Ambient
    vec3 ambient =
        vec3(0.25);

    // Diffuse
    vec3 norm =
        normalize(Normal);

    vec3 lightDir =
        normalize(lightPos - FragPos);

    float diff =
        max(dot(norm, lightDir), 0.0);

    vec3 diffuse =
        diff * vec3(0.8);

    // Simple color
    vec3 objectColor =
        vec3(0.65, 0.65, 0.7);

    vec3 result =
        (ambient + diffuse) *
        objectColor;

    FragColor =
        vec4(result, 1.0);
}