#version 330 core
uniform mat4 PVM;
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 c;
out vec3 opos;
out vec3 out_c;
void main() {
	gl_Position = PVM*vec4(pos.x, pos.y, pos.z, 1.0);
	gl_PointSize = 13.0f;
	opos = pos;
	out_c = c;
}