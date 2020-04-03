// Version du GLSL
#version 430 core

uniform mat4 MVP;
layout (triangles) in;
layout (triangle_strip, max_vertices = 5) out;

in vec3 tes_color[];
in float u[];
in float v[];
out vec3 geom_color;

// Paramètre du trimming
float u0 = 0.0;
float v0 = 0.0;
float rayon = 0.25;

// Les nouveaux sommets liés au trimming : a est le sommet différent des autres, b et c les deux autres sommets de départ, d = trimming _|_ [a;b] ; e = trimming _|_ [a;c]
vec4 a, b, c, d, e;

bool isTrimmed ( float u, float v )
{
    if ( 0.33 < u && u < 0.67 && 0.33 < v && v < 0.67 )
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
    uint nb_sommets_trimmed = 0;
    uint combinaison_trimmming = 0;

    if ( isTrimmed ( u[0], v[0] ) ) { nb_sommets_trimmed++ ; combinaison_trimmming += 4 ; }
    if ( isTrimmed ( u[1], v[1] ) ) { nb_sommets_trimmed++ ; combinaison_trimmming += 2 ; }
    if ( isTrimmed ( u[2], v[2] ) ) { nb_sommets_trimmed++ ; combinaison_trimmming += 1 ; }

    if ( nb_sommets_trimmed == 0 )
    {
        for ( int i = 0; i < gl_in.length(); i++ )
        {
            gl_Position = MVP * gl_in[i].gl_Position;
            geom_color = tes_color[i];
            EmitVertex();
        }
        EndPrimitive();
    }
    else if ( nb_sommets_trimmed < 3 )
    {
        geom_color = tes_color[0];

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
