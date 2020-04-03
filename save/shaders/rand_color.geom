// Version du GLSL
#version 430 core

uniform mat4 MVP;
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec3 geom_color;

vec3 rand ( float seed ) // La couleur est uniforme dans la même primitive
{
    float r = fract( 17 * float( seed ) / 23 );
    float g = fract( 41 * float( seed ) / 47 );
    float b = fract( 91 * float( seed ) / 97 );
    return vec3 ( r, g, b );
}

void main(void)
{
    // Par primitive de départ
    geom_color = rand( float(gl_PrimitiveIDIn) );
    // Par primitive après tesselation
    geom_color = rand( float(gl_PrimitiveIDIn) + 97 * ( length(gl_in[0].gl_Position.xz) + length(gl_in[1].gl_Position.xz) + length(gl_in[2].gl_Position.xz) ) );

    // Boucle qui est toujours la même
    for ( int i = 0; i < gl_in.length(); i++ )
    {
        gl_Position = MVP * gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}


