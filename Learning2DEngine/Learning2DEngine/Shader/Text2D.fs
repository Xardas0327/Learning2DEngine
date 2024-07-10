#version 330 core
in vec2 TextureCoords;

out vec4 color;

uniform sampler2D characterTexture;
uniform vec3 characterColor;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(characterTexture, TextureCoords).r);
    color = vec4(characterColor, 1.0) * sampled;
}  