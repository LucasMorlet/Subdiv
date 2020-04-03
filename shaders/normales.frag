// Version GLSL
#version 430

in vec2 coords_tex;
in vec2 coords_tex_locales;
uniform sampler2D normal_map;

in vec3 tangente;
in vec3 binormale;
in vec3 normale;

out vec4 color;

void display_normal_map_locale()
{
    vec3 norm_map = 2*texture2D( normal_map, coords_tex_locales.st ).xyz - vec3 (1.0f, 1.0f, 1.0f);
    vec3 new_normale = normalize(
                norm_map.x * tangente +
                norm_map.y * binormale +
                norm_map.z * normale
    );
    color = vec4 ( new_normale, 1.0f );
}

void display_normal_map_globale()
{
    vec3 norm_map = 2*texture2D( normal_map, coords_tex.st ).xyz - vec3 (1.0f, 1.0f, 1.0f);
    vec3 new_normale = normalize(
                norm_map.x * tangente +
                norm_map.y * binormale +
                norm_map.z * normale
    );
    color = vec4 ( new_normale, 1.0f );
}

void main()
{
    color = vec4 ( normale, 1.0f );
    //color = vec4 ( abs(normale), 1.0f );
    //color = vec4 ( abs(tangente), 1.0f );
    //color = vec4 ( abs(binormale), 1.0f );

    //display_normal_map_locale();
    //display_normal_map_globale();
}
