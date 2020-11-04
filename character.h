#pragma once
#include "texture.h"

class Character {

public:
	enum Sprite { left, right, jump, fall };
	GLfloat posX;
	GLfloat posY;
	int size;
	int health;
	int movementSpeed;
	GLfloat velocity;
	bool dead, gravityActive = false;
	int deathAnimation;
	GLfloat getPosX() {
		return posX;
	}

	GLfloat getPosY() {
		return posY;
	}

	int getSize() {
		return size;
	}

	bool isDead()
	{
		return dead;
	}
	
	void setPosition(GLfloat posX, GLfloat posY)
	{
		this->posX = posX;
		this->posY = posY;
	}
	
};