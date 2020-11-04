#pragma once
#include "character.h"
#include <Windows.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
#include <vector>
#include <stdlib.h>


class Shinigami : public Character
{

private:
	int moveLeft = 10;
	int moveRight = 10;
	int movementSpeed = 1;
	int rndMovement;
	bool attack;
	GLfloat playerX, playerY;
	std::map<char, std::vector<std::vector<int>>>* world;
	GLuint txr_idle = texture().loadPNG("textures/shinigami/idle.png");
	GLuint txr_attack = texture().loadPNG("textures/shinigami/attack.png");
public:


	Shinigami(int posX, int posY)
	{
		this->posX = posX;
		this->posY = posY;		
		this->size = 15;
		this->health = 100;
		dead = false;
		velocity = 0.5;
	}

	void draw(std::map<char, std::vector<std::vector<int>>>* world) {
		this->world = world; //no need to update this everytime.
		if (attack)
		{
			glBindTexture(GL_TEXTURE_2D, txr_attack);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, txr_idle);
		}
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

		glPushMatrix();
			glColor3f(0.9, 0.4, 0.6);
			glBegin(GL_QUADS);
				glVertex2f(posX, posY + size + 2);
				glVertex2f(posX, posY + size + 4);
				glVertex2f(posX + (size * health / 100), posY + size + 4);
				glVertex2f(posX + (size * health / 100), posY + size + 2);
			glEnd();
		glPopMatrix();
		move();
	}

	void updateWorld(GLfloat playerX, GLfloat playerY)
	{
		this->playerX = playerX;
		this->playerY = playerY;
	}

	void move() 
	{
		if (abs(posX-playerX) < 70 && (abs(posY - playerY) < 60 ))
		{
			attack = true;
			if (posX < playerX && collision(posX + movementSpeed, posY, false) != 1)
			{
				posX += movementSpeed;
			}
			else if (posX > playerX && collision(posX - movementSpeed, posY, false) != 1)
			{
				posX -= movementSpeed;
			}
		}
		else
		{
			attack = false;
		}

		gravity();

	}

	void die()
	{
		printf("shinigami ded");
		dead = true;
	}

	void damage(int d)
	{
		if (health - d <= 0)
		{
			die();
		}
		else
		{
			health -= d;
		}
	}




	void gravity()
	{

		if (collision(posX, posY-velocity, true) != 1) //if not jumping and not colliding with platform.
		{
			posY -= velocity;
			if (velocity < 6)
			{
				velocity = velocity * 1.1;
			}
		}


	}

	//code - No collision: 0, Platform '#': 1, Spikes '^': 2, Boosters '!': 3.
	int collision(GLfloat x, GLfloat y, bool stay)
	{

		for (int i = 0; i < world->at('#').size(); i++)
		{
			if (x + size > world->at('#')[i][0] && x < world->at('#')[i][1] && y + size > world->at('#')[i][2] && y < world->at('#')[i][3])
			{
				if (stay)
				{
					//posY = world->at('#')[i][3];
				}
				return 1;
			}
		}
		for (int i = 0; i < world->at('^').size(); i++)
		{
			if (x + size > world->at('^')[i][0] && x < world->at('^')[i][1] && y + size > world->at('^')[i][2] && y < world->at('^')[i][3])
			{
				die();
				return 2;
			}
		}

		for (int i = 0; i < world->at('!').size(); i++)
		{
			if (x + size > world->at('!')[i][0] && x < world->at('!')[i][1] && y + size > world->at('!')[i][2] && y < world->at('!')[i][3])
			{
				return 3;
			}
		}
		return 0;
	}
};