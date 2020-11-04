#pragma once
#include <Windows.h>
#include <GL\glew.h>


class Camera {
private:
	int strobeCounter;
	float moveDisplacment;
public:

	bool hit;
	int hitCounter;

	Camera()
	{
		strobeCounter = 0;
	}

	void setCamera(GLfloat x, GLfloat y) {

		if (hit && hitCounter < 10)
		{
			if (hitCounter % 2 == 0) {
				glTranslatef(x + 5, y, 0);
			}
			else {
				glTranslatef(x - 5, y, 0);
			}
			hitCounter++;
		}
		else if( hit && hitCounter >= 10 )
		{
			hit = false;			
		}
		else{
			glTranslatef(x, y, 0); //transleates world.
		}		
	}

	void move()
	{
		moveDisplacment;
	}

	void strobe()
	{
		if (strobeCounter >= 0 && strobeCounter < 200)
		{
			glTranslatef(strobeCounter-300, -100, 0);
		}

		else if (strobeCounter >= 200 && strobeCounter < 400)
		{
			glTranslatef(-strobeCounter+500, 0, 0);
		}

		else if (strobeCounter >= 400 && strobeCounter < 600)
		{
			glTranslatef(strobeCounter - 700, -strobeCounter/2 + 300, 0);

			if (strobeCounter >= 599)
			{
				strobeCounter = 0;
			}
		}
		strobeCounter++;
	}

	void setHit() {
		hit = true;
		hitCounter = 0;
	};
};