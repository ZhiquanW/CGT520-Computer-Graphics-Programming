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

#include "Surf.h"


// Triangles
static const std::string tri_vs("triangle_vs.glsl");
static const std::string tri_fs("triangle_fs.glsl");
GLuint tri_shader_program = -1;
GLuint tri_vao = -1;

//camera and viewport
float camangle = 0.0f;
glm::vec3 campos(0.0f, 1.0f, 2.0f);
float aspect = 1.0f;
GLint triVBOId;
float vertices[] = {
	// 位置              // 颜色
	 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
	-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
	 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
};

void draw_gui(){
   //glUseProgram(mesh_shader_program);
   static bool first_frame = true;
   ImGui_ImplGlut_NewFrame();

   ImGui::Begin("VAO Surf", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

   ImGui::SliderFloat3("Cam Pos", &campos[0], -20.0f, +20.0f);
   ImGui::SliderFloat("Cam Angle", &camangle, -180.0f, +180.0f);

   ImGui::End();

   ImGui::Render();
   first_frame = false;
}
GLuint create_tri_vbo() {

	
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	return VBO;
}

GLuint create_tri_vao() {
	unsigned int indices[] = { // 注意索引从0开始! 
	0, 1, 2, // 第一个三角形
	};
	
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	triVBOId = create_tri_vbo();
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	const GLint pos_loc = 0;
	glEnableVertexAttribArray(pos_loc);
	glVertexAttribPointer(pos_loc, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	const GLint col_loc = 1;
	glEnableVertexAttribArray(col_loc);
	glVertexAttribPointer(col_loc, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0); //unbind the vao	
	return vao;
}


void draw_tri() {
	glUseProgram(tri_shader_program);
	glBindBuffer(GL_ARRAY_BUFFER, triVBOId);
	float* ptr = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	// 如果指针为空（映射后的），更新VBO
	if (ptr) {
		for (int i = 0; i < 18; ++i) {
			std::cout << ptr[i] << " ";
		}std::cout << std::endl;
		for (int i = 0; i < 18; i+=6){
			ptr[i] += 0.01;
			ptr[i + 1] += 0.01;
		}
		glUnmapBufferARB(GL_ARRAY_BUFFER); // 使用之后解除映射
	}
	glBindVertexArray(tri_vao);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}



// glut display callback function.
// This function gets called every time the scene gets redisplayed 
void display()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear the back buffer
   
   glm::mat4 V = glm::lookAt(campos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f))*glm::rotate(camangle, glm::vec3(0.0f, 1.0f, 0.0f));
   glm::mat4 P = glm::perspective(80.0f, aspect, 0.1f, 100.0f); //not affine


   draw_tri();
   draw_gui();
   glutSwapBuffers();
}

void idle()
{
   glutPostRedisplay();

   const int time_ms = glutGet(GLUT_ELAPSED_TIME);
   float time_sec = 0.001f*time_ms;
}

void printGlInfo()
{
   std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
   std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
   std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
   std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

void initOpenGl()
{
   glewInit();

   glEnable(GL_DEPTH_TEST);
   glEnable(GL_POINT_SPRITE);       // allows textured points
   glEnable(GL_PROGRAM_POINT_SIZE); //allows us to set point size in vertex shader
   glClearColor(0.65f, 0.65f, 0.65f, 1.0f);

   tri_shader_program = InitShader(tri_vs.c_str(), tri_fs.c_str());
   tri_vao = create_tri_vao();
   ImGui_ImplGlut_Init(); // initialize the imgui system
}

// glut callbacks need to send keyboard and mouse events to imgui
void keyboard(unsigned char key, int x, int y)
{
   ImGui_ImplGlut_KeyCallback(key);
}

void keyboard_up(unsigned char key, int x, int y)
{
   ImGui_ImplGlut_KeyUpCallback(key);
}

void special_up(int key, int x, int y)
{
   ImGui_ImplGlut_SpecialUpCallback(key);
}

void passive(int x, int y)
{
   ImGui_ImplGlut_PassiveMouseMotionCallback(x, y);
}

void special(int key, int x, int y)
{
   ImGui_ImplGlut_SpecialCallback(key);
}

void motion(int x, int y)
{
   ImGui_ImplGlut_MouseMotionCallback(x, y);
}

void mouse(int button, int state, int x, int y)
{
   ImGui_ImplGlut_MouseButtonCallback(button, state);
}

void reshape(int w, int h)
{
   glViewport(0, 0, w, h);
   aspect = (float)w / h;
}

int main(int argc, char **argv)
{
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

   initOpenGl();

   //Enter the glut event loop.
   glutMainLoop();
   glutDestroyWindow(win);
   return 0;
}