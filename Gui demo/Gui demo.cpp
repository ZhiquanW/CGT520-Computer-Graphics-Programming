#include <windows.h>
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glext.h>

#include <glm/glm.hpp> //This header defines basic glm types (vec3, mat4, etc)

//These headers define matrix transformations
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <glm/gtc/type_ptr.hpp> //This header defines helper functions (e.g. value_ptr(...)) for passing vectors and matrices to shaders

#include "InitShader.h"    //Functions for loading shaders from text files
#include "LoadMesh.h"      //Functions for creating OpenGL buffers from mesh files
#include "LoadTexture.h"   //Functions for creating OpenGL textures from image files
#include "imgui_impl_glut.h" //Functions for using imgui UI in glut programs

static const std::string vertex_shader("gui_demo_vs.glsl");
static const std::string fragment_shader("gui_demo_fs.glsl");

GLuint shader_program = -1;
GLuint texture_id = -1; //Texture map for fish

static const std::string mesh_name = "Amago0.obj";
static const std::string texture_name = "AmagoT.bmp";
MeshData mesh_data;

float angle = 0.0f;
float scale = 1.0f;
bool enableClearScreen = true;
bool enableDepthTesting = true;
float contrast = 1;
float height = 0.01;
float speed = 1;
float texture_speed = 0;
float float_speed = 0;
float clearColor[4] = { 0 };
//Draw the ImGui user interface
void draw_gui()
{
   ImGui_ImplGlut_NewFrame();

   ImGui::Begin("Debug menu");
   //uncomment the following line to create a sliders which changes the viewing angle and scale
   ImGui::SliderFloat("View angle", &angle, -180.0f, +180.0f);
   ImGui::SliderFloat("Scale", &scale, -10.0f, +10.0f);
  
   ImGui::End();
   ImGui::Begin("Zhiquan Wang");
   if (ImGui::Button("Reset Rotation", ImVec2(100, 20))) {
	   angle = 0.0;
   }
   ImGui::Checkbox("Clear Screen", &enableClearScreen);
   ImGui::Checkbox("Depth Testing", &enableDepthTesting);
   ImGui::ColorEdit4("Color", clearColor);
   ImGui::SliderFloat("Contrast", &contrast, 0.0f, 2.0f);
   ImGui::SliderFloat("Height", &height, 0, 1);
   ImGui::SliderFloat("speed", &speed, 0, 10);
   ImGui::SliderFloat("texture_speed", &texture_speed, 0, 5);
   ImGui::SliderFloat("float_speed", &float_speed, 0, 10);
   ImGui::End();
   static bool show_test = false;
   ImGui::ShowTestWindow(&show_test);

   ImGui::Render();
 }

// glut display callback function.
// This function gets called every time the scene gets redisplayed 
void display()
{
	if (enableClearScreen) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear the back buffer

		//Ans:if disable, fish rendered with different rotation because the previous color still stored in the buffer
	}
	if (enableDepthTesting) {
		glEnable(GL_DEPTH_TEST);
	}
	else {
		glDisable(GL_DEPTH_TEST);
		//Ans, if disable, the inside and the back of the fish can been seen because the pixel is not renderer from back to from
	}

	glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
	glm::mat4 M = glm::rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(glm::vec3(scale * mesh_data.mScaleFactor));
	glm::mat4 V = glm::lookAt(glm::vec3(0.0f, 1.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 P = glm::perspective(40.0f, 1.0f, 0.1f, 100.0f);


	glUseProgram(shader_program);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	int PVM_loc = glGetUniformLocation(shader_program, "PVM");
	if (PVM_loc != -1)
	{
		glm::mat4 PVM = P * V * M;
		glUniformMatrix4fv(PVM_loc, 1, false, glm::value_ptr(PVM));
	}
	
	int P_loc = glGetUniformLocation(shader_program, "P");
	if (P_loc != -1)
	{
		glUniformMatrix4fv(P_loc, 1, false, glm::value_ptr(P));
	}

	int VM_loc = glGetUniformLocation(shader_program, "VM");
	if (VM_loc != -1)
	{
		glm::mat4 VM = V * M;
		glUniformMatrix4fv(VM_loc, 1, false, glm::value_ptr(VM));
	}

	int contrast_fra_loc = glGetUniformLocation(shader_program, "contrast_fra");
	if (contrast_fra_loc != -1) {
		glUniform1f(contrast_fra_loc, contrast);
	}
	   int height_vs_loc = glGetUniformLocation(shader_program, "height");
   if (height_vs_loc != -1) {
	   glUniform1f(height_vs_loc, height);
   }
   int speed_vs_loc = glGetUniformLocation(shader_program, "speed");
   if (speed_vs_loc != -1) {
	   glUniform1f(speed_vs_loc, speed);
   }
   int ts_vs_loc = glGetUniformLocation(shader_program, "texture_speed");
   if (ts_vs_loc != -1) {
	   glUniform1f(ts_vs_loc, texture_speed);
   }
   int fs_vs_loc = glGetUniformLocation(shader_program, "float_speed");
   if (fs_vs_loc != -1) {
	   glUniform1f(fs_vs_loc, float_speed);
   }
	int tex_loc = glGetUniformLocation(shader_program, "diffuse_tex");
	if (tex_loc != -1)
	{
		glUniform1i(tex_loc, 0); // we bound our texture to texture unit 0
	}

	glBindVertexArray(mesh_data.mVao);
	glDrawElements(GL_TRIANGLES, mesh_data.mSubmesh[0].mNumIndices, GL_UNSIGNED_INT, 0);
	//For meshes with multiple submeshes use mesh_data.DrawMesh(); 

	draw_gui();

	glutSwapBuffers();
}

void idle()
{
	glutPostRedisplay();

   const int time_ms = glutGet(GLUT_ELAPSED_TIME);
   float time_sec = 0.001f*time_ms;

   //Pass time_sec value to the shaders
   int time_loc = glGetUniformLocation(shader_program, "time");
   if (time_loc != -1)
   {
      glUniform1f(time_loc, time_sec);
   }

}

void reload_shader()
{
   GLuint new_shader = InitShader(vertex_shader.c_str(), fragment_shader.c_str());

   if (new_shader == -1) // loading failed
   {
      glClearColor(1.0f, 0.0f, 1.0f, 0.0f); //change clear color if shader can't be compiled
   }
   else
   {
      glClearColor(0.35f, 0.35f, 0.35f, 0.0f);

      if (shader_program != -1)
      {
         glDeleteProgram(shader_program);
      }
      shader_program = new_shader;
   }
}

void printGlInfo()
{
   std::cout << "Vendor: "       << glGetString(GL_VENDOR)                    << std::endl;
   std::cout << "Renderer: "     << glGetString(GL_RENDERER)                  << std::endl;
   std::cout << "Version: "      << glGetString(GL_VERSION)                   << std::endl;
   std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION)  << std::endl;
}

void initOpenGl()
{
   glewInit();

   glEnable(GL_DEPTH_TEST);

   reload_shader();

   //mesh and texture to be rendered
   mesh_data = LoadMesh(mesh_name);
   texture_id = LoadTexture(texture_name);
}

// glut callbacks need to send keyboard and mouse events to imgui
void keyboard(unsigned char key, int x, int y)
{
   ImGui_ImplGlut_KeyCallback(key);

   switch(key)
   {
      case 'r':
      case 'R':
         reload_shader();     
      break;
   }
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
   ImGui_ImplGlut_PassiveMouseMotionCallback(x,y);
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


int main (int argc, char **argv)
{
   //Configure initial window state
   glutInit(&argc, argv); 
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
   glutInitWindowPosition (5, 5);
   glutInitWindowSize (640, 640);
   int win = glutCreateWindow ("GUI demo");

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

   initOpenGl();
   ImGui_ImplGlut_Init(); // initialize the imgui system

   //Enter the glut event loop.
   glutMainLoop();
   glutDestroyWindow(win);
   return 0;		
}