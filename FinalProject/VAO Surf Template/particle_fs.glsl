#version 330 core
out vec4 FragColor;
in vec3 opos;
void main() {
	//FragColor = vec4(3.0f / 256.0f, 161.0f / 256.0f, opos.x / 200.0f, 1.0f);
	float r = distance(gl_PointCoord, vec2(0.5));
	if (r > 0.5f) {
		discard;
	}
	if (0.46f<=r && r <= 0.5f) {
		gl_FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	} else {
		gl_FragColor = vec4(30.0f / 256.0f, 161.0f / 256.0f, opos.x / 200.0f, 1.0f);

	}
}