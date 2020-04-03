// Version du GLSL
#version 430 core

in vec3 geom_color;

layout (location = 0) out vec4 diffuseColor;

void main()
{
    diffuseColor = vec4( geom_color, 1.0f );
}
