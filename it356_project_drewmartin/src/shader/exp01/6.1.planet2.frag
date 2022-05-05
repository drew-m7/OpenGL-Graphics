#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D aTexture;

void main()
{    
    vec4 texColor = texture(aTexture, TexCoords);
    //vec4 texColor = texture(aTexture, TexCoords);

    FragColor = texColor;
}

