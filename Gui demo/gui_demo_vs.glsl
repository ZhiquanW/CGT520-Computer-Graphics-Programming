#version 400            
uniform mat4 PVM;
uniform mat4 P;
uniform mat4 VM;
uniform float texture_speed;

uniform float time;
uniform float height;
uniform float speed;
in vec3 pos_attrib;
in vec2 tex_coord_attrib;
in vec3 normal_attrib;

out vec3 pos;
out vec2 tex_coord;  

void main(void)
{
	vec3 tmp = pos_attrib;
	tmp[1] += sin(tmp[0] + speed * time) * height;
	gl_Position = P * VM * vec4(tmp, 1.0);
	tex_coord = tex_coord_attrib*cos(time * texture_speed);
	pos = pos_attrib;
}