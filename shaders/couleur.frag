// Version du GLSL
#version 430 core

in vec3 geom_color;
out vec4 couleur;

void main()
{
    couleur = vec4( geom_color, 1.0f );
}
