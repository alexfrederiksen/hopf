#version 330 core

out vec4 color;

uniform vec3 fiberColor;

void main()
{
    color = vec4(fiberColor, 1.0);
}


