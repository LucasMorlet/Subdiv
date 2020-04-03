// Version GLSL
#version 430

struct Lumiere
{
    vec3 position; // View position
    vec3 couleur;
    int puissance;
};
uniform Lumiere lesLumieres[10];

struct Materiau
{
    float coeff_ambiant;
    float coeff_speculaire;
    float coeff_diffus;
    int hardness;
};
uniform Materiau materiau;

// Par sommets
in vec3 view_pos;
in vec3 normale;
in vec3 geom_color;

//layout (location = 0)
out vec4 color;

void main()
{
    color = vec4 ( 0.0f, 0.0f, 0.0f, 1.0f );
    for ( int i = 0 ; i < 10 ; i++ )
    {
        // On extrait les infos de la lumière courante
        vec3 light_col = lesLumieres[i].couleur;
        vec3 light_pos = lesLumieres[i].position - view_pos;
        float distance = length ( light_pos );
        float power = lesLumieres[i].puissance / ( distance * distance );
        light_pos = normalize ( light_pos );

        // Lumière ambiante
        vec3 ambiant = materiau.coeff_ambiant * lesLumieres[i].couleur * geom_color;

        // Lumière diffuse
        float val_diff = max ( 0, dot( normale, light_pos ));
        vec3 diffuse = materiau.coeff_diffus * power * val_diff * light_col * geom_color;

        // Lumière spéculaire
        float val_spec = max ( 0, dot( normale, normalize ( normalize (-view_pos) + light_pos ) ));
        vec3 specular = materiau.coeff_speculaire * power * light_col * pow( val_spec, materiau.hardness );

        // Le blending final
        color.xyz += ambiant + diffuse + specular;
    }

    // Gamma correction ???
    // Gestion de l'atténuation avec la distance

    /* Toon Shading *
    float intervalle = 4;
    color *= intervalle;
    color = vec4 ( int(color.x)/intervalle, int(color.y)/intervalle, int(color.z)/intervalle, 1.0f);
    // Fin Toon Shading */
}
