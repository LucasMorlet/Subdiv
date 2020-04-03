// Version du GLSL
#version 430 core

uniform mat4 MVP;
layout (triangles) in;
layout (triangle_strip, max_vertices = 15) out;

in
in vec3 tes_color[];
out vec3 geom_color;

void main(void)
{
    // Pour chaque face
    vec4 u = gl_in[1].gl_Position - gl_in[0].gl_Position;
    vec4 v = gl_in[2].gl_Position - gl_in[0].gl_Position;
    vec3 normale = normalize( cross (u.xyz, v.xyz) );
    // Pour chaque nouvelle primitives
    for ( int j = 1 ; j <= 5 ; j++ )
    {
        vec3 deplacement = j * 0.1f * normale;
        // Pour chaque vertex
        for ( int i = 0; i < 3; i++ )
        {
            gl_Position = MVP * vec4( gl_in[i].gl_Position.xyz + deplacement.xyz, gl_in[i].gl_Position.w );
            geom_color = tes_color[i];
            EmitVertex();
        }
        EndPrimitive();
    }
}
