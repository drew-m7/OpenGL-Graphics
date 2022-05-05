#version 460 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    vec3 specular;
    float shininess;
};
uniform Material material;

in vec2 TexCoords;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;
in vec3 FragPos;
in vec3 Normal;
uniform vec3 viewPos;

uniform sampler2D aTexture;

void main()
{    
    //vec4 texColor = texture(aTexture, TexCoords);
    //vec4 texColor = texture(aTexture, TexCoords);

    //FragColor = texColor;

    //ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    //diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    //specular
    //no specular because the sun is more of a natural ambient light with some diffuse

    vec3 result = ambient + diffuse;
    FragColor = vec4(result, 1.0);
}