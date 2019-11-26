#version 330 core
uniform mat4 PVM;
layout(location = 0) in vec3 pos;
out vec3 opos;

void main() {
	gl_Position = PVM*vec4(pos.x, pos.y, pos.z, 1.0);
	gl_PointSize = 20.0f;
	opos = pos;
}