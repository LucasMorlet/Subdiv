// Version du GLSL
#version 430 core

uniform mat4 MVP;
layout (triangles) in;
layout (triangle_strip, max_vertices = 5) out;

in vec3 tes_color[];
in vec2 tex_coords[];
in vec2 tex_coords_locales[];
out vec3 geom_color;

// Paramètre du trimming
float u0 = 0.0;
float v0 = 0.0;
float rayon = 0.0;

// Les nouveaux sommets liés au trimming : a est le sommet différent des autres, b et c les deux autres sommets de départ, d = trimming _|_ [a;b] ; e = trimming _|_ [a;c]
vec4 a, b, c, d, e;

bool isTrimmed ( float u, float v )
{
    //return false;
    if ( pow( u - u0, 2 ) + pow ( v - v0, 2 ) < rayon * rayon )
    {
        return true;
    }
    return false;
}

float calculIntersection ( float a_u, float a_v, float b_u, float b_v )
{
    // Calcul du discriminant
    float A = pow ( b_u - a_u, 2 ) + pow ( b_v - a_v, 2 );
    float B = 2 * ( ( b_u - a_u ) * ( a_u - u0 ) + ( b_v - a_v ) * ( a_v - v0 ) );
    float C = pow ( a_u - u0, 2 ) + pow ( a_v - v0, 2 ) - rayon*rayon;
    float d = sqrt ( B*B - 4*A*C );

    // On teste laquelle des deux solutions de l'intersection cercle/droite appartient au segment
    float t = ( -B + d ) / ( 2*A );
    if ( 0 <= t && t <= 1.0 )
    {
        return t;
    }
    else
    {
        return ( -B - d ) / ( 2*A );
    }
}

void main( void )
{
    /* Gaussienne */
    uint patch_1 = 10;
    uint patch_2 = 11;
    uint patch_3 = 17;
    uint patch_4 = 18;
    uint patch_5 = 18;
    uint patch_6 = 18;
    // Fin gaussienne */

    /* Tetris
    uint patch_1 = 37;
    uint patch_2 = 43;
    uint patch_3 = 47;
    uint patch_4 = 50;
    uint patch_5 = 54;
    uint patch_6 = 56;
    // Fin tetris */

    if ( gl_PrimitiveIDIn == patch_1 )
    {
        rayon = 0.4;
        u0 = 1.0;
        v0 = 1.0;
    }
    if ( gl_PrimitiveIDIn == patch_2 )
    {
        rayon = 0.4;
        v0 = 1.0;
    }
    if ( gl_PrimitiveIDIn == patch_3 )
    {
        rayon = 0.4;
        u0 = 1.0;
    }
    if ( gl_PrimitiveIDIn == patch_4 )
    {
        rayon = 0.4;
    }
    if ( gl_PrimitiveIDIn == patch_5 )
    {
        rayon = 0.4;
    }
    if ( gl_PrimitiveIDIn == patch_6 )
    {
        rayon = 0.4;
    }

    if ( gl_PrimitiveIDIn == patch_1 || gl_PrimitiveIDIn == patch_2 || gl_PrimitiveIDIn == patch_3 || gl_PrimitiveIDIn == patch_4 || gl_PrimitiveIDIn == patch_5 || gl_PrimitiveIDIn == patch_6 )
    {
        uint nb_sommets_trimmed = 0;
        uint combinaison_trimmming = 0;
        float u[3];
        float v[3];
        u[0] = tex_coords_locales[0][0];
        u[1] = tex_coords_locales[1][0];
        u[2] = tex_coords_locales[2][0];
        v[0] = tex_coords_locales[0][1];
        v[1] = tex_coords_locales[1][1];
        v[2] = tex_coords_locales[2][1];

        if ( isTrimmed ( u[0], v[0] ) ) { nb_sommets_trimmed++ ; combinaison_trimmming += 4 ; }
        if ( isTrimmed ( u[1], v[1] ) ) { nb_sommets_trimmed++ ; combinaison_trimmming += 2 ; }
        if ( isTrimmed ( u[2], v[2] ) ) { nb_sommets_trimmed++ ; combinaison_trimmming += 1 ; }

        if ( nb_sommets_trimmed == 0 )
        {
            for ( int i = 0; i < gl_in.length(); i++ )
            {
                gl_Position = MVP * gl_in[i].gl_Position;
                geom_color = vec3 ( 1.0f, 0.0f, 0.0f );
                EmitVertex();
            }
            EndPrimitive();
        }
        else if ( nb_sommets_trimmed < 3 )
        {
            float coeff_d, coeff_e;

            if ( combinaison_trimmming == 3 || combinaison_trimmming == 4 )
            {
                a = gl_in[0].gl_Position;
                b = gl_in[1].gl_Position;
                c = gl_in[2].gl_Position;
                coeff_d = calculIntersection ( u[0], v[0], u[1], v[1] );
                coeff_e = calculIntersection ( u[0], v[0], u[2], v[2] );
            }
            else if ( combinaison_trimmming == 2 || combinaison_trimmming == 5 )
            {
                a = gl_in[1].gl_Position;
                b = gl_in[2].gl_Position;
                c = gl_in[0].gl_Position;
                coeff_d = calculIntersection ( u[1], v[1], u[2], v[2] );
                coeff_e = calculIntersection ( u[1], v[1], u[0], v[0] );
            }
            else // combinaison == 1
            {
                a = gl_in[2].gl_Position;
                b = gl_in[0].gl_Position;
                c = gl_in[1].gl_Position;
                coeff_d = calculIntersection ( u[2], v[2], u[0], v[0] );
                coeff_e = calculIntersection ( u[2], v[2], u[1], v[1] );
            }
            d = ( 1 - coeff_d ) * a + coeff_d * b;
            e = ( 1 - coeff_e ) * a + coeff_e * c;

            // La création de la primitive
            if ( nb_sommets_trimmed == 1 )
            {
                geom_color = vec3 ( 0.0f, 0.5f, 0.0f );
                a = ( b + c ) / 2;
                gl_Position = MVP * c;
                EmitVertex();
                gl_Position = MVP * e;
                EmitVertex();
                gl_Position = MVP * a;
                EmitVertex();
                gl_Position = MVP * d;
                EmitVertex();
                gl_Position = MVP * b;
                EmitVertex();
            }
            else
            {
                geom_color = vec3 ( 0.0f, 0.5f, 1.0f );
                gl_Position = MVP * a;
                EmitVertex();
                gl_Position = MVP * d;
                EmitVertex();
                gl_Position = MVP * e;
                EmitVertex();
            }
            EndPrimitive();
        }
    }
    else
    {
        for ( int i = 0; i < gl_in.length(); i++ )
        {
            gl_Position = MVP * gl_in[i].gl_Position;
            geom_color = vec3 ( 0.5f, 0.5f, 0.5f );
            EmitVertex();
        }
        EndPrimitive();
    }
}
