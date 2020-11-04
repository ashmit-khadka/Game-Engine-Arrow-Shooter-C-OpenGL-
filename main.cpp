#include "main.h"
#include <fstream>
#include "menu.h"
std::vector<std::vector<int>> platforms;
Game game;
Camera camraMain;
Menu menu;
GLuint myTexture = 0;
int mouseX = 0, mouseY = 0;
bool clickBuffer = false;
using namespace std;


void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity(); // resets matrix.

	if (game.getMode() == 1) //Game actvie
	{
		camraMain.setCamera(-game.getPlayer().getPosX(), -game.getPlayer().getPosY());
		game.draw();
	}
	else if (game.getMode() == 0) //Game idle
	{
		camraMain.strobe();
		game.draw();
		menu.draw(0, game.getPlayer().getPoints(), game.getTopScore());
	}
	else if (game.getMode() == 2) //Game Over - Player dead
	{
		camraMain.strobe();
		game.draw();
		menu.draw(2, game.getPlayer().getPoints(), game.getTopScore());
	}
	else //Game Over - Player won
	{
		camraMain.strobe();
		game.draw();
		menu.draw(3, game.getPlayer().getPoints(), game.getTopScore());
	}
	glutSwapBuffers();
}

void reshape(int w, int h) //called when then window is resized, w & h are the size of the current window
{
	game.setWindowDimension(w, h);
	glViewport(0, 0, (GLsizei)w, (GLsizei)h); //identify which part of the display is to be viewed.
	glMatrixMode(GL_PROJECTION); //switch matrix mode to the projection matrix.
	glLoadIdentity(); // resets matrix (the projection martix).
	gluOrtho2D(-100, 100, -100, 100); //re define the cordinates system for projecting.
	//this will prevent primatives from becomming streched when resizing the window.
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();								// Reset The Modelview Matrix

}

void timer(int)
{
	glutPostRedisplay();
	glutTimerFunc(1000 / 60, timer, 0);
}

void keyDownHandler(unsigned char key, int x, int y)
{
	if (key == 'w')
	{
		game.getPlayer().jump();
	}
	if (key == 'a')
	{
		game.getPlayer().moveLeft(true);
	}
	if (key == 'd')
	{
		game.getPlayer().moveRight(true);
	}
	if (key == 'e')
	{
		camraMain.setHit();
	}
	if (key == 'f')
	{
		game.toggleTutorial();
	}
}

void keyUpHandler(unsigned char key, int x, int y)
{
	if (key == 'w')
	{
	}
	if (key == 'a')
	{
		game.getPlayer().moveLeft(false);
	}
	if (key == 'd')
	{
		game.getPlayer().moveRight(false);
	}
}

void mouseHandler(int button, int state, int x, int y)
{

	if (button == GLUT_LEFT_BUTTON && game.getMode() == 1 && !clickBuffer)
	{

		game.getPlayer().bowAttack();

	}
	clickBuffer = false;

	if (button == GLUT_LEFT_BUTTON && game.getMode() == 0)
	{
		int action = menu.getMouseAction(x, y);
		if (action == 1) 
		{
			game.setMode(1);
			game.toggleTutorial();
		}
		clickBuffer = true;
	}

	if (button == GLUT_LEFT_BUTTON && game.getMode() == 2)
	{
		int action = menu.getMouseAction(x, y);
		if (action == 1)
		{
			game.reset();
			game.setWindowDimension(glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT));
			game.setMode(1);
		}
		clickBuffer = true;
	}

	if (button == GLUT_LEFT_BUTTON && game.getMode() == 3)
	{
		int action = menu.getMouseAction(x, y);
		if (action == 1)
		{
			game.reset();
			game.setWindowDimension(glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT));
			game.setMode(1);
		}
		clickBuffer = true;
	}

	game.setMouseCoordinates(x, glutGet(GLUT_WINDOW_HEIGHT) - y);

}

void mouseMotionHandler(int x, int y)
{
	game.setMouseCoordinates(x, glutGet(GLUT_WINDOW_HEIGHT) - y);
}

void init() {
	game = Game(1000, 3000, &camraMain);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
}

int main(int argc, char* argv[]) {

	// Initialize GLUT
	glutInit(&argc, argv);	// Set up some memory buffers for our display
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);	// Set the window size
	glutInitWindowSize(800, 800);	// Create the window with the title
	glutCreateWindow("5010B-CW");	// Bind the two functions (above) to respond when necessary
	glutDisplayFunc(display);
	glutTimerFunc(0, timer, 0);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyDownHandler);
	glutKeyboardUpFunc(keyUpHandler);	
	glutMouseFunc(mouseHandler);
	glutMotionFunc(mouseMotionHandler);
	init();
	glEnable(GL_TEXTURE_2D);


	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW error");
		return 1;
	}
	glutMainLoop();
	return 0;
}





