#pragma once
#include "texture.h"

class Arrow 
{
private:

	float velocityH, velocityV, angle, posX, posY, time, dy, dx, g = 1;
	int power;
	bool peaked;
	float size;

	GLuint* arrowSprite;

public:

	Arrow(int power, float x, float y, int dy, int dx, std::map<char, std::vector<std::vector<int>>>* world, GLuint *arrowSprite)
	{

		this->dy = dy;
		this->dx = dx;
		this->power = power;
		posX = x;
		posY = y;
		time = 1;
		size = 5;
		peaked = false;
		
		angle = atan2(dy, dx);
		int u = power/10;
		velocityH = abs(u * cos(angle));
		velocityV = abs(u * sin(angle));
		this->arrowSprite = arrowSprite;

	}

	void draw()
	{
		time+=0.1;

		if (dy > 0)
		{
			posY += (velocityV * time) + (0.5 * -g * time * time);
		}
		else
		{
			posY -= (velocityV * time);
		}

		if (dx > 0)
		{
			posX += velocityH;
		}
		else
		{
			posX -= velocityH;
		}		

		glBindTexture(GL_TEXTURE_2D, *arrowSprite);
			glPushMatrix();
			glColor3f(1.0, 1.0, 1.0);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0, 0.0); glVertex2f(posX, posY);
				glTexCoord2f(0.0, 1.0); glVertex2f(posX, posY + size);
				glTexCoord2f(1.0, 1.0); glVertex2f(posX + size, posY + size);
				glTexCoord2f(1.0, 0.0); glVertex2f(posX + size, posY);
			glEnd();
			glPopMatrix();
		glBindTexture(GL_TEXTURE_2D, 0);


	}

	float getPosX()
	{
		return posX;
	}

	float getPosY()
	{
		return posY;
	}

	int getSize()
	{
		return size;
	}
	
	int getPower()
	{
		return power;
	}

};