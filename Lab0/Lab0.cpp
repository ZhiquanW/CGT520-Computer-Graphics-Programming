#include <windows.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glext.h>

#include <iostream>


void blackBackground(int v) {
	std::cout << "background color changed to black.\n";
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glFinish();
	glutTimerFunc(5 * 1000, blackBackground, 0);
}
// glut display callback function.
// This function gets called every time the scene gets redisplayed
void display(){
   //Clear the screen to the color previously specified in the glClearColor(...) call.
   glClear(GL_COLOR_BUFFER_BIT);

   //Here is where you would write code to draw something.

   glFinish();
}

void reshape(int width, int height){
	std::cout << "New Window Size :" << glutGet(GLUT_WINDOW_WIDTH) << " - " << glutGet(GLUT_WINDOW_HEIGHT) << std::endl;
}
void specialKeyboard(int key, int x, int y) {
	std::cout << "PRESSED" << std::endl;

	std::cout << "special key : " << key << ", x: " << x << ", y: " << y << std::endl;
	//titlebar_pos = render_part_pos - border_part;
	int ini_window_pos_x = glutGet(GLUT_WINDOW_X) - glutGet(GLUT_WINDOW_BORDER_WIDTH);
	int ini_window_pos_y = glutGet(GLUT_WINDOW_Y) - glutGet(GLUT_WINDOW_BORDER_HEIGHT);

	switch (key) {
	case GLUT_KEY_LEFT:
		glutPositionWindow(ini_window_pos_x - 1, ini_window_pos_y);
		break;
	case GLUT_KEY_RIGHT:
		glutPositionWindow(ini_window_pos_x + 1, ini_window_pos_y);
		break;
	case GLUT_KEY_UP:
		glutPositionWindow(ini_window_pos_x, ini_window_pos_y - 1);
		break;
	case GLUT_KEY_DOWN:
		glutPositionWindow(ini_window_pos_x, ini_window_pos_y + 1);
		break;
	case GLUT_KEY_F1:
		glutHideWindow();		
		break;
	case GLUT_KEY_F5:
		glutSetCursor(GLUT_CURSOR_CYCLE);
		break;
	case GLUT_KEY_F6:
		glutSetCursor(GLUT_CURSOR_WAIT);
		break;
	case GLUT_KEY_F7:
		glutSetCursor(GLUT_CURSOR_HELP);
		break;
	case GLUT_KEY_F8:
		glutSetCursor(GLUT_CURSOR_LEFT_RIGHT);
		break;
	}
	
}

void specialKeyboardUp(int key, int x, int y) {
	std::cout << "RELEASED" << std::endl;

}
// glut keyboard callback function.
// This function gets called when an ASCII key is pressed
void keyboard(unsigned char key, int x, int y){
	std::cout << "PRESSED" << std::endl;

	if (glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
		std::cout << "Shift Active - ";
	}
	else {
		std::cout << "Shift Inactive - ";
	}
	std::cout << "key : " << key << ", x: " << x << ", y: " << y << std::endl;
	int ini_window_size_w = glutGet(GLUT_WINDOW_WIDTH);
	int ini_window_size_h = glutGet(GLUT_WINDOW_HEIGHT);
	
	switch (key) {
	case 'f':
		if (glutGet(GLUT_WINDOW_WIDTH) != (GLUT_SCREEN_WIDTH) &&
			glutGet(GLUT_WINDOW_HEIGHT) != glutGet(GLUT_SCREEN_HEIGHT)) {
			glutFullScreen();
		}
		else {
			glutReshapeWindow(glutGet(GLUT_INIT_WINDOW_WIDTH),
				glutGet(GLUT_INIT_WINDOW_HEIGHT));
		}
		break;

	case '+':
		glutReshapeWindow(ini_window_size_w + 10, ini_window_size_h + 10);
		std::cout << "New Window Size :" << glutGet(GLUT_WINDOW_WIDTH) << " - " << glutGet(GLUT_WINDOW_HEIGHT) << std::endl;
		break;
	case '-':
		glutReshapeWindow(ini_window_size_w - 10, ini_window_size_h - 10);
		std::cout << "New Window Size :" << glutGet(GLUT_WINDOW_WIDTH) << " - " << glutGet(GLUT_WINDOW_HEIGHT) << std::endl;
		break;
	}
	
}
void keyboardUp(unsigned char key, int x, int y) {
	std::cout << "RELEASED" << std::endl;

}

// glut keyboard callback function.
// This function gets called when a mouse button is pressed
void mouse(int button, int state, int x, int y){
   std::cout << "button : "<< button << ", state: " << state << ", x: " << x << ", y: " << y << std::endl;
}
void mouseWheel(int wheel, int dir, int x, int y) {
	int ini_window_size_w = glutGet(GLUT_WINDOW_WIDTH);
	int ini_window_size_h = glutGet(GLUT_WINDOW_HEIGHT);
	int d = dir > 0 ? 1 : -1;
	glutReshapeWindow(ini_window_size_w + d * 10, ini_window_size_h + d * 10);
	std::cout << "New Window Size :" << glutGet(GLUT_WINDOW_WIDTH) << " - " << glutGet(GLUT_WINDOW_HEIGHT) << std::endl;
}
//Print out information about the OpenGL version supported by the graphics driver.	
void printGlInfo(){
   std::cout << "Vendor: "       << glGetString(GL_VENDOR)                    << std::endl;
   std::cout << "Renderer: "     << glGetString(GL_RENDERER)                  << std::endl;
   std::cout << "Version: "      << glGetString(GL_VERSION)                   << std::endl;
   std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION)  << std::endl;
   std::cout << "Screen Width: " << glutGet(GLUT_SCREEN_WIDTH) << std::endl;
   std::cout << "Screen Height: " << glutGet(GLUT_SCREEN_HEIGHT) << std::endl;
   std::cout << "Window Width: " << glutGet(GLUT_WINDOW_WIDTH) << std::endl;
   std::cout << "Window Height: " << glutGet(GLUT_WINDOW_HEIGHT) << std::endl;
   std::cout << "Numerber of bits per pixel in R: " << glutGet(GLUT_WINDOW_RED_SIZE) << std::endl;
   std::cout << "Numerber of bits per pixel in G: " << glutGet(GLUT_WINDOW_GREEN_SIZE) << std::endl;
   std::cout << "Numerber of bits per pixel in B: " << glutGet(GLUT_WINDOW_BLUE_SIZE) << std::endl;
   std::cout << "Numerber of bits per pixel in A: " << glutGet(GLUT_WINDOW_ALPHA_SIZE) << std::endl;
}

//C++ programs start executing in the main() function.
int main (int argc, char **argv){
   //Configure initial window state
   glutInit(&argc, argv); // can pass command line args to glut
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowPosition (5, 5);
   glutInitWindowSize (512, 512);
   int win = glutCreateWindow ("Zhiquan Wang");

   printGlInfo();

   //Register callback functions with glut. 
   glutDisplayFunc(display); 
   glutMouseFunc(mouse);
   glutMouseWheelFunc(mouseWheel);
   glutKeyboardFunc(keyboard);
   glutKeyboardUpFunc(keyboardUp);
   glutSpecialFunc(specialKeyboard);
   glutSpecialUpFunc(specialKeyboardUp);
   glutReshapeFunc(reshape);
   glutTimerFunc(5 * 1000, blackBackground, 0);
   //Set the color the screen gets cleared to.
   glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

   //Enter the glut event loop.
   glutMainLoop();
   glutDestroyWindow(win);
   return 0;		
}

