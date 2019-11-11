#include <windows.h>


#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glext.h>

#include <iostream>

float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};
void display();
void printGlInfo();



//C++ programs start executing in the main() function.
int main (int argc, char **argv)
{
   //Configure initial window state
   glutInit(&argc, argv); // can pass command line args to glut
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowPosition (5, 5);
   glutInitWindowSize (512, 512);
   int win = glutCreateWindow ("CGT 520 System Info");

   printGlInfo();

   //Register callback functions with glut. 
   glutDisplayFunc(display); 

   //Set the color the screen gets cleared to.
   glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
   //Enter the glut event loop.
   glutMainLoop();
   glutDestroyWindow(win);
   return 0;		
}

// glut display callback function.
// This function gets called every time the scene gets redisplayed
void display() {
	//Clear the screen to the color previously specified in the glClearColor(...) call.
	glClear(GL_COLOR_BUFFER_BIT);
	//Here is where you would write code to draw something.
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glFinish();
}

//Print out information about the OpenGL version supported by the graphics driver.	
void printGlInfo() {
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}
