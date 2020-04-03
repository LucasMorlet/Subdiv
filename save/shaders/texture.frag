// Version du GLSL
#version 430 core

uniform sampler2D color_map;
in vec2 tex_coords;

layout (location = 0) out vec4 diffuseColor;

void main()
{
    diffuseColor = texture2D( color_map, tex_coords.st );
}
