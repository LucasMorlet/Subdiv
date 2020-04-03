// Version du GLSL
#version 430 core

uniform sampler2D color_map;
uniform sampler2D normal_map;
in vec2 coords_tex;

out vec4 couleur;

void main()
{
    couleur = texture2D( color_map, coords_tex.st );
}
