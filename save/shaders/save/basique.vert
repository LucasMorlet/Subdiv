// Version du GLSL
#version 430 core

in vec3 position;
in vec3 couleur;
in vec3 normale;
out vec3 vert_color;

void main()
{
    gl_Position = vec4( position, 1.0 );
    vert_color = couleur;
}
