#version 330 core
layout (location = 0) in vec2 postion;
layout (location = 1) in vec2 textureCoords;

out vec2 TexCoords;

uniform bool  chaos;
uniform bool  confuse;
uniform bool  shake;
uniform float time;

void main()
{
    gl_Position = vec4(postion, 0.0f, 1.0f);
    if (chaos)
    {
        float strength = 0.3;
        vec2 pos = vec2(textureCoords.x + sin(time) * strength, textureCoords.y + cos(time) * strength);        
        TexCoords = pos;
    }
    else if (confuse)
    {
        TexCoords = vec2(1.0 - textureCoords.x, 1.0 - textureCoords.y);
    }
    else
    {
        TexCoords = textureCoords;
    }
    if (shake)
    {
        float strength = 0.01;
        gl_Position.x += cos(time * 10) * strength;        
        gl_Position.y += cos(time * 15) * strength;        
    }
}  