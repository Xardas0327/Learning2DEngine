#version 330 core
in vec2 TextureCoords;

out vec4 color;

uniform bool isUseTexture;
uniform sampler2D spriteTexture;
uniform vec3 spriteColor;

void main()
{    
    color = vec4(spriteColor, 1.0);
    if(isUseTexture)
    {
        color*= texture(spriteTexture, TextureCoords);
    }
}