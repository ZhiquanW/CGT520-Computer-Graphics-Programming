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
#include "FluidParameter.h"
#include "RestrictionBox.h"
#include "FluidSolver.h"
#include "Vector3.h"
//fluid solver parameters

glm::vec3 left_bottom_front(0,0,0);
glm::vec3 right_top_back(300,400,300);
const int particle_num =1000;
float rest_density = -1000;
float gas_constant = 2000;
float viscosity = 250;
float kernel_radius = 16;
float mass = 65;
float bound_damping = 0.5f;
float gravity =  120000;
int frame_num = 400;
float time_interval = 0.001;
//camera and viewport
float camangle = 100.0f;
glm::vec3 campos(150.0f, 200.0f, -600.0f);
glm::vec3 camtar(150.0f, 200.0f, 0.0f);
float aspect = 1.0f;

// particles 2d
float particle_pos[particle_num * 3] = {0};
// particles
static const std::string particle_vs("particle_vs.glsl");
static const std::string particle_fs("particle_fs.glsl");
GLuint particle_shader_program = -1;
GLuint particle_vbo = -1;
GLuint particle_vao = -1;
// restriction box
static const std::string restrictionbox_vs("restrictionbox_vs.glsl");
static const std::string restrictionbox_fs("restrictionbox_fs.glsl");
GLuint restrictionbox_shader_program = -1;
GLuint restrictionbox_vao = -1;
GLuint restrictionbox_ebo = -1;
// fluid solver
FluidParameter tmp_paras(particle_num, mass, kernel_radius, rest_density, viscosity, gas_constant, 0.01,gravity);
RestrictionBox tmp_box(Vector3(left_bottom_front), Vector3(right_top_back), bound_damping);
FluidSolver tmp_solver(tmp_paras, tmp_box, time_interval);

void draw_gui();
GLuint create_particle_vbo();
GLuint create_particle_vao();
void draw_particle(glm::mat4);
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
void update_params();


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
	glm::mat4 V = glm::lookAt(campos, camtar, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(camangle, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 P = glm::perspective(80.0f, aspect, 0.1f, 10000.0f); //not affine
	update_params();
	draw_particle(P*V);
	draw_gui();
	tmp_box.draw(restrictionbox_vao, P * V, restrictionbox_shader_program);
	glutSwapBuffers();
}

void initOpenGl() {
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POINT_SPRITE);       // allows textured points
	glEnable(GL_PROGRAM_POINT_SIZE); //allows us to set point size in vertex shader
	glClearColor(0.65f, 0.65f, 0.65f, 1.0f);
	tmp_solver.initialize_particles(Vector3(0, 0, 0), Vector3(200, 200,200), 16);
	// init elementes in the scene
	particle_shader_program = InitShader(particle_vs.c_str(), particle_fs.c_str());
	particle_vao = create_particle_vao();
	restrictionbox_shader_program = InitShader(restrictionbox_vs.c_str(), restrictionbox_fs.c_str());
	restrictionbox_vao = tmp_box.create_vao();
	ImGui_ImplGlut_Init(); // initialize the imgui system
}

void draw_gui() {
	//glUseProgram(mesh_shader_program);
	static bool first_frame = true;
	ImGui_ImplGlut_NewFrame();
	ImGui::Begin("Camera Params", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::SliderFloat3("Cam Pos", &campos[0], -1000.0f, +1000.0f);
	ImGui::SliderFloat("Cam Angle", &camangle, -180.0f, +180.0f);
	ImGui::End();
	ImGui::Begin("Restriction Box", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::SliderFloat3("left_bottom_front",&left_bottom_front[0],0,500);
	ImGui::SliderFloat3("right_top_back", &right_top_back[0], 0, 500);

	ImGui::End();
	//ImGui::Begin("Fluid Solver Params", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	////ImGui::SliderInt("particle_num", &particle_num, 0, 10000);
	//ImGui::SliderFloat("Time Interval", &time_interval, 0, 0.01);
	//ImGui::SliderFloat("Kernal Radius", &kernel_radius, 1, 32);
	//ImGui::SliderFloat("Mass", &mass, 0, 100);
	//ImGui::SliderFloat("Gravity", &gravity, 0, 100);
	//ImGui::SliderFloat("Rest Density", &rest_density, 0, 300);
	//ImGui::SliderFloat("Gas Constant", &gas_constant,0,300);
	//ImGui::SliderFloat("Viscosity", &viscosity, 0, 500);
	//ImGui::SliderFloat("Bound Damping", &bound_damping, 0, 1);
	//ImGui::End();
	ImGui::Render();
	first_frame = false;
}
void update_params() {
	tmp_solver.set_time_interval(time_interval);
	tmp_paras.set_core_radius(kernel_radius);
	tmp_paras.set_particle_mass(mass);
	tmp_paras.set_gravity_acceleration_coefficient(gravity);
	tmp_paras.set_rest_density(rest_density);
	tmp_paras.set_gas_constant(gas_constant);
	tmp_paras.set_viscosity_coefficient(viscosity);
	tmp_box.set_bound_damping_coefficient(bound_damping);
	tmp_solver.set_params(tmp_paras);
	tmp_solver.set_restriction_box(tmp_box);
}
void draw_particle(glm::mat4 m) {
	glUseProgram(particle_shader_program);
	glBindBuffer(GL_ARRAY_BUFFER, particle_vbo);
	tmp_solver.simulate_particles();		
	float* ptr = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	vector<float> tmp_pos = tmp_solver.get_pos();
	if (ptr) {
		std::copy(tmp_pos.begin(), tmp_pos.end(), ptr);
		glUnmapBufferARB(GL_ARRAY_BUFFER);
	}
	int PVM_loc = glGetUniformLocation(particle_shader_program, "PVM");
	if (PVM_loc != -1) {
		glUniformMatrix4fv(PVM_loc, 1, false, glm::value_ptr(m));
	}
	glBindVertexArray(particle_vao);
	glDrawArrays(GL_POINTS, 0, particle_num*3);
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