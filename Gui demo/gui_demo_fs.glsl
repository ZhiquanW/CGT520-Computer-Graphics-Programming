#version 400

uniform sampler2D diffuse_tex;
uniform float contrast_fra;
uniform float time;
uniform float float_speed;
out vec4 fragcolor;           
in vec2 tex_coord;
in vec3 pos;
void main(void)
{   
   vec4 tex_color = texture(diffuse_tex, tex_coord);
   fragcolor = tex_color * contrast_fra + cos(tex_coord[0]*10+ float_speed*time)/10;
}




















