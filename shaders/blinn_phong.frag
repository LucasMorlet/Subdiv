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

// Les textures
uniform sampler2D color_map;
uniform sampler2D normal_map;

// Par sommets
in vec3 view_pos;
in vec3 normale;
in vec3 tangente;
in vec3 binormale;
in vec2 coords_tex;
in vec2 coords_tex_locales;

// La couleur en sortie
out vec4 output_color;

void main()
{
    vec3 eye = normalize ( /*vec3 ( 0.0f, 0.0f, 0.0f )*/ - view_pos );
    output_color = vec4 ( 0.0f, 0.0f, 0.0f, 1.0f );

    // Color map
    vec3 couleur = texture2D( color_map, coords_tex.st ).xyz;

    // Normal map
    vec3 new_normale;
    if ( false )
    {
        vec3 norm_map = 2*texture2D( normal_map, coords_tex_locales.st ).xyz - vec3 (1.0f, 1.0f, 1.0f);
        new_normale = normalize(
                    norm_map.x * tangente +
                    norm_map.y * binormale +
                    norm_map.z * normale
        );
    }
    else
    {
        new_normale = normale;
    }

    // Lumière ambiante
    output_color.xyz += materiau.coeff_ambiant * couleur;

    // Pour chaque lumière de la scène
    for ( int i = 0 ; i < 1 ; i++ )
    {
        // On extrait les infos de la lumière courante
        vec3 light_col = lesLumieres[i].couleur;
        vec3 light_pos = normalize ( lesLumieres[i].position - view_pos );
        float distance = length ( lesLumieres[i].position - view_pos );
        float power = lesLumieres[i].puissance / ( distance * distance );

        // Lumière diffuse
        float val_diff = max ( 0, dot( new_normale, light_pos ));
        //val_diff = max ( val_diff, dot( -normale, light_pos ) );
        vec3 diffuse = materiau.coeff_diffus * power * val_diff * light_col * couleur;

        // Lumière spéculaire
        float val_spec = max ( 0, dot( new_normale, ( eye + light_pos ) / 2 ) );
        //val_spec = max ( val_spec, dot( normalize ( normalize (-view_pos) + light_pos ), -normale )); // Si la normale est orientée vers l'extérieur
        vec3 specular = materiau.coeff_speculaire * power * light_col * pow( val_spec, materiau.hardness );

        // Le blending final
        output_color.xyz += diffuse + specular;
    }

    // Gamma correction ???
    // Gestion de l'atténuation avec la distance

    /* Toon Shading *
    float intervalle = 4;
    color *= intervalle;
    color = vec4 ( int(color.x)/intervalle, int(color.y)/intervalle, int(color.z)/intervalle, 1.0f);
    // Fin Toon Shading */
}
