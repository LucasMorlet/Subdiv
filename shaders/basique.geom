// Version du GLSL
#version 430 core

uniform vec3 lumiere;
uniform mat4 MVP;
uniform mat4 MV;
uniform mat3 tiMV; // transpose(inverse(MV))
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 tes_tangente [];
in vec3 tes_binormale[];
in vec3 tes_normale  [];
in vec2 tex_coords   [];
in vec2 tex_coords_locales[];

out vec3 view_pos;
out vec3 normale;
out vec3 tangente;
out vec3 binormale;
out vec2 coords_tex;
out vec2 coords_tex_locales;

void main(void)
{
    // On envoie les infos des différents sommets
    for ( int i = 0 ; i < gl_in.length ; i++ )
    {
        gl_Position         = MVP * gl_in[i].gl_Position;
        view_pos            = (MV * gl_in[i].gl_Position).xyz;
        tangente            = normalize ( tiMV * tes_tangente [i] );
        binormale           = normalize ( tiMV * tes_binormale[i] );
        normale             = normalize ( tiMV * tes_normale  [i] );
        coords_tex          = tex_coords[i];
        coords_tex_locales  = tex_coords_locales[i];
        EmitVertex();
    }
    EndPrimitive();
}
