// Version du GLSL
#version 430 core

// Ce qu'on reçoit par les VertexAttributes
in vec3 position;
in vec2 texture;

// Ce qu'on envoie, en plus de la position, à la suite du pipeline
out vec2 tex_coords;

void main()
{
    gl_Position = vec4( position, 1.0 );
    tex_coords = texture;
}
