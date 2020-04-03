// Version du GLSL
#version 430 core

uniform mat4 MVP;
layout (triangles) in;
layout (points, max_vertices = 3) out;

in vec3 tes_color[];
out vec3 geom_color;

void main(void)
{
    for ( int i = 0; i < gl_in.length(); i++)
    {
        gl_Position = MVP * gl_in[i].gl_Position;
        gl_PointSize = 4.0f;
        geom_color = tes_color[i];
        EmitVertex();
    }
    EndPrimitive();
}
