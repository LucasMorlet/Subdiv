// Version du GLSL
#version 430 core

uniform mat4 MVP;
uniform mat4 MV;
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 tes_color[];

out vec3 view_pos;
out vec3 normale;
out vec3 tangente;
out vec3 binormale;
out vec2 coords_tex;

void main(void)
{
    // Calcul de la normale de la face triangulaire
    vec3 p0 = (MV * gl_in[0].gl_Position).xyz;
    vec3 p1 = (MV * gl_in[1].gl_Position).xyz;
    vec3 p2 = (MV * gl_in[2].gl_Position).xyz;

    tangente  = normalize ( p1 - p0 );
    binormale = normalize ( p2 - p0 );
    normale   = normalize ( cross ( tangente, binormale ) );

    /*
    vec3 eye  = - p0;
    if ( length ( normale + eye ) < length ( normale - eye ) )
    {
        normale = -normale;
    }
    */

    // On envoie les infos des différents sommets
    gl_Position = MVP * gl_in[0].gl_Position;
    view_pos = p0;  
    EmitVertex();

    gl_Position = MVP * gl_in[1].gl_Position;
    view_pos = p1; 
    EmitVertex();

    gl_Position = MVP * gl_in[2].gl_Position;
    view_pos = p2; 
    EmitVertex();

    EndPrimitive();
}
