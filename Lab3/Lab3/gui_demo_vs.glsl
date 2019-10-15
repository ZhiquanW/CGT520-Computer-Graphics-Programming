#version 400            
uniform mat4 PVM;
uniform mat4 P;
uniform mat4 VM;

uniform float time;

in vec3 pos_attrib;
in vec2 tex_coord_attrib;
in vec3 normal_attrib;

out vec2 tex_coord;  

void main(void)
{
   gl_Position = P*VM*vec4(pos_attrib, 1.0);
   tex_coord = tex_coord_attrib;
}