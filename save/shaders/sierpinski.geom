// Version du GLSL
#version 430 core

uniform mat4 MVP;
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 tes_color[];
in uint numero_affichage[];
out vec3 geom_color;

bool affichage ( uint a, uint b, uint c )
{
    if ( a == 0 && b == 1 )
    {
        return true;
    }
    else if ( a == 1 && b == 2 )
    {
        return true;
    }
    else if ( a == 2 && b == 3 )
    {
        return true;
    }
    else if ( a == 3 && b == 4 )
    {
        return true;
    }
    else if ( a == 5 && b == 6 )
    {
        return true;
    }
    else if ( a == 7 && b == 8 )
    {
        return true;
    }
    else if ( a == 9 && b == 10 )
    {
        return true;
    }
    else if ( a == 10 && b == 11 )
    {
        return true;
    }
    else if ( a == 12 && b == 13 )
    {
        return true;
    }
    else if ( a == 0 && b == 2 )
    {
        return true;
    }
    else if ( a == 2 && b == 4 )
    {
        return true;
    }
    else if ( a == 9 && b == 11 )
    {
        return true;
    }
    else
    {
        return false;
    }
}

void main(void)
{
    uint num1 = numero_affichage[0];
    uint num2 = numero_affichage[1];
    uint num3 = numero_affichage[2];

    // a < b < c
    uint a = min ( num1, min ( num2, num3 ) );
    uint b;
    uint c = max ( num1, max ( num2, num3 ) );
    if ( num1 != a && num1 != c )
    {
        b = num1;
    }
    else if ( num2 != a && num2 != c )
    {
        b = num2;
    }
    else
    {
        b = num3;
    }


    if ( affichage(a, b, c) )
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
