// Version du GLSL
#version 430 core

//uniform vec3 camera;
uniform vec3 lumiere;
uniform mat4 MVP;
uniform mat4 MV;
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 tes_color[];
out vec3 geom_color;
out vec3 view_pos;
out vec3 normale;

void main(void)
{
    // Calcul de la normale de la face triangulaire
    vec4 p0 = MV * gl_in[0].gl_Position;
    vec4 p1 = MV * gl_in[1].gl_Position;
    vec4 p2 = MV * gl_in[2].gl_Position;
    normale = normalize( cross ( p1.xyz - p0.xyz, p2.xyz - p0.xyz ) );

    // On envoie les infos des différents sommets
    gl_Position = MVP * gl_in[0].gl_Position;
    geom_color = tes_color[0];
    view_pos = p0.xyz;
    EmitVertex();

    gl_Position = MVP * gl_in[1].gl_Position;
    geom_color = tes_color[1];
    view_pos = p1.xyz;
    EmitVertex();

    gl_Position = MVP * gl_in[2].gl_Position;
    geom_color = tes_color[2];
    view_pos = p2.xyz;
    EmitVertex();

    EndPrimitive();
}
