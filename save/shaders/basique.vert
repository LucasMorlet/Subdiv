// Version du GLSL
#version 430 core

in vec3 position;
uniform int time;

void main()
{
    gl_Position = vec4( position, 1.0 );
}
