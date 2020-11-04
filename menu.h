#pragma once
#include <Windows.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <string>


class Menu {

private:
	int mouseX, mouseY;


public:
	Menu()
	{

	}
	void draw(int status, int playerScore, int topScore)
	{
		std::string stats;
		char buffer[200];		
		glLoadIdentity();
		glPushMatrix();

			glColor3f(1.0, 1.0, 1.0);
			glBegin(GL_QUADS);
				glVertex2f(-30, -10);
				glVertex2f(-30, 10);
				glVertex2f(30, 10);
				glVertex2f(30, -10);
			glEnd();

			glColor4f(0.0, 0.0, 0.0, 0.4);
			stats = sprintf_s(buffer, "Play", playerScore, topScore);
			glRasterPos2f(-5, 0);
			glutBitmapString(GLUT_BITMAP_9_BY_15, (const unsigned char*)buffer);

			glBegin(GL_QUADS);
				glVertex2f(-30, 30);
				glVertex2f(-30, 60);
				glVertex2f(30, 60);
				glVertex2f(30, 30);
			glEnd();

			if (status == 2)
			{
				stats = sprintf_s(buffer, "GAME OVER!\n\nYour score: %d\nTop score: %d", playerScore, topScore);
			}
			else if (status == 3)
			{
				stats = sprintf_s(buffer, "YOU ESCAPED THE FOREST!\n\nYour score: %d\nTop score: %d", playerScore, topScore);
			}
			else
			{
				stats = sprintf_s(buffer, "5010B - Graphics 1\nCoursework\nAshmit Khadka");
			}

			glColor3f(1.0, 1.0, 1.0);
			glRasterPos2f(-25, 50);
			glutBitmapString(GLUT_BITMAP_9_BY_15, (const unsigned char*)buffer);

		glPopMatrix();
	}



	int getMouseAction(int mouseX, int mouseY)
	{
		mouseY = 800 - mouseY;
		if (mouseX >= 400-120 && mouseX <= 400+120 && mouseY >= 400-40 && mouseY <= 400+40)
		{
			return 1;
		}
		return 0;
	}

};