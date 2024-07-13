#version 330 core
layout (location = 0) in vec2 postion;
layout (location = 1) in vec2 textureCoords;

out vec2 TextureCoords;

void main()
{
    gl_Position = vec4(postion, 0.0, 1.0);
    TextureCoords = textureCoords;
} 