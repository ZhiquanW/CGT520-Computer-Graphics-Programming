#version 330 core
uniform mat4 PVM;
layout(location = 0) in vec3 pos;
void main() {
	gl_Position = PVM * vec4(pos.x, pos.y, pos.z, 1.0);
	//gl_Position = PVM * vec4(1.0f, 1.0f, 1.0f, 1.0f);
	gl_PointSize = 10.0f;

}