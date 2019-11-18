#include <windows.h>
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glext.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "InitShader.h"
#include "imgui_impl_glut.h"
#include "VAO Surf.h"
#include "Vector3.h"
#define vec Vector3

//fluid solver parameters
vec left_bottom_front = vec();
vec right_top_back = vec(300, 400, 300);
int particle_num = 2000;
float rest_density = -1000;
float gas_constant = 2000;
float viscosity = 250;
float kernel_radius = 16;
float mass = 65;
float bound_damping = 0.5;
float gravity = 12000 * 9.8;
int frame_num = 400;
float time_interval = 0.001;
//camera and viewport
float camangle = 0.0f;
glm::vec3 campos(0.0f, 1.0f, 2.0f);
float aspect = 1.0f;

// particles 2d
float particle_pos[] = {
	0.0f,0.0f,0.0f,
	1.0f,1.0f,1.0f,
	2.0f,0.0f,2.0f
};
static const std::string particle_vs("particle_vs.glsl");
static const std::string particle_fs("particle_fs.glsl");
GLuint particle_shader_program = -1;
GLuint particle_vbo = -1;
GLuint particle_vao = -1;

void draw_gui();
GLuint create_particle_vbo();
GLuint create_particle_vao();
void draw_particle();
void display();
void idle();
void printGlInfo();
void initOpenGl();
void keyboard(unsigned char key, int x, int y);
void keyboard_up(unsigned char key, int x, int y);
void special_up(int key, int x, int y);
void passive(int x, int y);
void special(int key, int x, int y);
void motion(int x, int y);
void mouse(int button, int state, int x, int y);
void reshape(int w, int h);



int main(int argc, char** argv) {
	//Configure initial window state
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(5, 5);
	glutInitWindowSize(640, 640);
	int win = glutCreateWindow("Zhiquan Wang");

	printGlInfo();

	//Register callback functions with glut. 
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutKeyboardUpFunc(keyboard_up);
	glutSpecialUpFunc(special_up);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutPassiveMotionFunc(motion);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	// init configuration
	initOpenGl();
	//Enter the glut event loop.
	glutMainLoop();
	glutDestroyWindow(win);
	return 0;
}

// glut display callback function.
// This function gets called every time the scene gets redisplayed 
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear the back buffer
	glm::mat4 V = glm::lookAt(campos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(camangle, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 P = glm::perspective(80.0f, aspect, 0.1f, 100.0f); //not affine
	draw_particle();
	draw_gui();
	glutSwapBuffers();
}

void initOpenGl() {
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POINT_SPRITE);       // allows textured points
	glEnable(GL_PROGRAM_POINT_SIZE); //allows us to set point size in vertex shader
	glClearColor(0.65f, 0.65f, 0.65f, 1.0f);
	// init elementes in the scene
	particle_shader_program = InitShader(particle_vs.c_str(), particle_fs.c_str());
	particle_vao = create_particle_vao();
	ImGui_ImplGlut_Init(); // initialize the imgui system
}

void draw_gui() {
	//glUseProgram(mesh_shader_program);
	static bool first_frame = true;
	ImGui_ImplGlut_NewFrame();
	ImGui::Begin("Camera Params", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::SliderFloat3("Cam Pos", &campos[0], -20.0f, +20.0f);
	ImGui::SliderFloat("Cam Angle", &camangle, -180.0f, +180.0f);
	ImGui::End();
	ImGui::Begin("Fluid Solver Params", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::SliderInt("particle_num", &particle_num, 0, 10000);
	ImGui::End();
	ImGui::Render();
	first_frame = false;
}
void draw_particle() {
	glUseProgram(particle_shader_program);
	//glBindBuffer(GL_ARRAY_BUFFER, particle_vbo);
	//float* ptr = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	//if (ptr) {
	//	for (int i = 0; i < 18; i += 6) {
	//		ptr[i] += 0.01;
	//		ptr[i + 1] += 0.01;
	//	}
	//	glUnmapBufferARB(GL_ARRAY_BUFFER); // 使用之后解除映射
	//}
	int PVM_loc = glGetUniformLocation(particle_shader_program, "PVM");
	if (PVM_loc != -1) {
		glm::mat4 V = glm::lookAt(campos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(camangle, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 P = glm::perspective(80.0f, aspect, 0.1f, 100.0f); //not affine
		glm::mat4 PVM = P * V;
		glUniformMatrix4fv(PVM_loc, 1, false, glm::value_ptr(PVM));
	}
	glBindVertexArray(particle_vao);
	glDrawArrays(GL_POINTS, 0, 3);
}
GLuint create_particle_vbo() {
	GLuint vbo = -1;
	// generate a buffer with id
	glGenBuffers(1, &vbo);
	// bind buffer to a specific type
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// copy data into buffer's memory
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_pos), particle_pos, GL_DYNAMIC_DRAW);
	return vbo;
}
GLuint create_particle_vao() {
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	particle_vbo = create_particle_vbo();
	const GLint pos_loc = 0;
	glEnableVertexAttribArray(pos_loc);
	glVertexAttribPointer(pos_loc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glBindVertexArray(0);
	return vao;
}

void idle() {
	glutPostRedisplay();
	const int time_ms = glutGet(GLUT_ELAPSED_TIME);
	float time_sec = 0.001f * time_ms;
}

void printGlInfo() {
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}
// glut callbacks need to send keyboard and mouse events to imgui
void keyboard(unsigned char key, int x, int y) {
	ImGui_ImplGlut_KeyCallback(key);
}

void keyboard_up(unsigned char key, int x, int y) {
	ImGui_ImplGlut_KeyUpCallback(key);
}

void special_up(int key, int x, int y) {
	ImGui_ImplGlut_SpecialUpCallback(key);
}

void passive(int x, int y) {
	ImGui_ImplGlut_PassiveMouseMotionCallback(x, y);
}

void special(int key, int x, int y) {
	ImGui_ImplGlut_SpecialCallback(key);
}
void motion(int x, int y) {
	ImGui_ImplGlut_MouseMotionCallback(x, y);
}

void mouse(int button, int state, int x, int y) {
	ImGui_ImplGlut_MouseButtonCallback(button, state);
}

void reshape(int w, int h) {
	glViewport(0, 0, w, h);
	aspect = (float)w / h;
}