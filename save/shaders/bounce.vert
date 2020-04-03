// Version du GLSL
#version 430 core

in vec3 position;
uniform int time;

layout ( std430, binding = 1 ) buffer buff_sommets
{
    float les_sommets[][3];
};

void main()
{
    float y = position.y + 0.75*(position.y + 0.25)*abs(0.5 - float(time)/1000);
    vec4 new_sommet = vec4 ( position.x, y, position.z, 1.0 );
    les_sommets[gl_VertexID][1] = y;
    gl_Position = new_sommet;
}
