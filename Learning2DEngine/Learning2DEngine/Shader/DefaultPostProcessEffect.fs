#version 330 core
in vec2 TextureCoords;

out vec4 color;

uniform sampler2D scene;

void main()
{   
    color =  texture(scene, TextureCoords);
}