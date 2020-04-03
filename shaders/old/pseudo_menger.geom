// Version du GLSL
#version 430 core

uniform mat4 MVP;
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 tes_color[];
in uint numero_affichage[];
out vec3 geom_color;


void main(void)
{
    /*
    uint num1 = numero_affichage[0];
    uint num2 = numero_affichage[1];
    uint num3 = numero_affichage[2];
    */

    uint num = min( numero_affichage[0], min( numero_affichage[1], numero_affichage[2] ) );

    //if ( num1 != 12 && num2 != 12 && num3 != 12 )
    if ( num != 10 /*&& num != 15 && num != 55 */ && num != 60 /*&& num != 30 && num != 31 && num != 39 && num != 40*/ )
    {
        for ( int i = 0; i < gl_in.length(); i++ )
        {
            gl_Position = MVP * gl_in[i].gl_Position;
            geom_color = tes_color[i];
            EmitVertex();
        }
        EndPrimitive();
    }
}
