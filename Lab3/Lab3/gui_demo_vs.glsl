#version 400            
uniform mat4 PVM;
uniform mat4 P;
uniform mat4 VM;
uniform mat4 M;
uniform float time;
uniform vec3 light_pos;
in vec3 pos_attrib;
in vec2 tex_coord_attrib;
in vec3 normal_attrib;

out vec2 tex_coord;
out vec3 nw;
out vec3 lw;
out vec3 pw;	
out float dis;

void main(void)
{
   gl_Position = P*VM*vec4(pos_attrib, 1.0);
   tex_coord = tex_coord_attrib;
   pw = vec3(M * vec4(pos_attrib, 1.0));
   lw = light_pos - pw;
   dis = length(lw);
   lw = vec3(normalize(lw));
   nw = vec3(normalize(M * vec4(normal_attrib, 0.0)));
  
}