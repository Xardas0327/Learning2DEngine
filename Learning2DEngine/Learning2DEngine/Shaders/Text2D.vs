#version 330 core
layout (location = 0) in vec2 postion;
layout (location = 1) in vec2 textCoords;

out vec2 TexCoords;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(postion, 0.0, 1.0);
    TexCoords = textCoords;
} 