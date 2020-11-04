#include "player.h"

Player::Player(int posX, int posY)
{
	this->posX = posX;
	this->posY = posY;
	size = 8;
	health = 100;
	keyLeft = false;
	keyRight = false;
	keyJump = false;
	jumpLock = false;
	velocity = 0.5;
	swordRange = 10;
	points = 0;
	keyAttack = false;
	dead = false;
	victory = false;
	sprite = right;
	runSpriteIdx = 0;
	topIdle = texture().loadPNG("textures/player/top.png");
	topAttack = texture().loadPNG("textures/player/attack.png");

	jumpSprite = texture().loadPNG("textures/player/jump.png");
	fallSprite = texture().loadPNG("textures/player/fall.png");
	arrowSprite = texture().loadPNG("textures/world/arrow.png");
	runSprite = {
		texture().loadPNG("textures/player/run1.png"),
		texture().loadPNG("textures/player/run2.png"),
		texture().loadPNG("textures/player/run3.png")
	};

}


void Player::updateWorld(std::map<char, std::vector<std::vector<int>>>* world, std::vector<Shinigami>* shinigamies, bool* platformSwitch, Camera * camera)
{
	this->world = world;
	this->shinigamies = shinigamies;
	this->platformSwitch = platformSwitch;
	this-> camera = camera;
}

void Player::draw(int &mouseX, int &mouseY)
{

	int halfSize = size / 2; //casted to int instead of float for efficiency 

	if (keyAttack)
	{
		mouseAngle = atan2((mouseY - windowY), (mouseX - windowX)) * 180 / 3.14;
		this->mouseX = mouseX; //To provide vertial velocity to arrow.
		this->mouseY = mouseY; //To provide vertial velocity to arrow.
		if (mouseX - windowX >= 0)
		{
			sprite = right;
		}
		else 
		{
			sprite = left;
		}
		addBowPower();
		drawProjection(bowPower, mouseAngle, posX + halfSize, posY + 6 + halfSize, mouseY - windowY, mouseX - windowX);

	}
	else
	{
		mouseAngle = 0;
		if (keyRight) { sprite = right; }
		if (keyLeft) { sprite = left; }

	}
	frameTimer++;

	if (!keyLeft && !keyRight)
	{
		runSpriteIdx = 2;
		frameTimer = 0;
	}
	else if (keyRight && frameTimer % 10 == 0)
	{
		runSpriteIdx++;
	}
	else if (keyLeft && frameTimer % 10 == 0)
	{
		runSpriteIdx--;
		if (runSpriteIdx < 0) { runSpriteIdx = 2; }
	}
	runSpriteIdx = runSpriteIdx % 3;
	if (keyJump)
	{
		glBindTexture(GL_TEXTURE_2D, jumpSprite);
	}
	else if (gravityActive)
	{
		glBindTexture(GL_TEXTURE_2D, fallSprite);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, runSprite.at(runSpriteIdx));
	}
	
	glPushMatrix();

		glColor3f(0.0, 0.0, 0.0);
		glBegin(GL_QUADS);
			if (sprite == right)
			{
				glTexCoord2f(0.0, 0.0); glVertex2f(posX, posY);
				glTexCoord2f(0.0, 1.0); glVertex2f(posX, posY + size);
				glTexCoord2f(1.0, 1.0); glVertex2f(posX + size, posY + size);
				glTexCoord2f(1.0, 0.0); glVertex2f(posX + size, posY);
			}
			else if (sprite == left)
			{
				glTexCoord2f(1.0, 0.0); glVertex2f(posX, posY);
				glTexCoord2f(1.0, 1.0); glVertex2f(posX, posY + size);
				glTexCoord2f(0.0, 1.0); glVertex2f(posX + size, posY + size);
				glTexCoord2f(0.0, 0.0); glVertex2f(posX + size, posY);
			}



		glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);

	//printf("mx: %d, wx: %d, my: %d, wy: %d, angle: %f\n", mouseX, windowX, mouseY, windowY, mouseAngle);
	if (keyAttack)
	{
		glBindTexture(GL_TEXTURE_2D, topAttack);

	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, topIdle);
	}
	glPushMatrix();
		glColor3f(0.0, 0.0, 0.0);
		glTranslatef(posX + halfSize, posY + 2 + halfSize, 0);
		if (keyAttack && sprite == left)
		{
			glRotatef(mouseAngle -180, 0, 0, 1);
		}
		else
		{
			glRotatef(mouseAngle, 0, 0, 1);
		}
		glBegin(GL_QUADS);
			if (sprite == right)
			{
				glTexCoord2f(0.0, 0.0); glVertex2f(-halfSize, -halfSize + 3);
				glTexCoord2f(0.0, 1.0); glVertex2f(-halfSize, halfSize + 3);
				glTexCoord2f(1.0, 1.0); glVertex2f(halfSize, halfSize + 3);
				glTexCoord2f(1.0, 0.0); glVertex2f(halfSize, -halfSize + 3);
			}
			else if (sprite == left)
			{
				glTexCoord2f(1.0, 0.0); glVertex2f(-halfSize, -halfSize + 3);
				glTexCoord2f(1.0, 1.0); glVertex2f(-halfSize, halfSize + 3);
				glTexCoord2f(0.0, 1.0); glVertex2f(halfSize, halfSize + 3);
				glTexCoord2f(0.0, 0.0); glVertex2f(halfSize, -halfSize + 3);
			}

		glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
	collision(posX, posY, true);
	drawArrows();
	movement();
	gravity();
	

}


void Player::drawProjection(float power, float a, float x, float y, int dy, int dx)
{
	int g = 1;
	a = atan2(dy, dx);
	int u = power / 10;
	int sined = sin(a);
	float vh = abs(u * cos(a));
	float vv = abs(u * sin(a));
	   	 
	bool peaked = false;		

	glPushMatrix();
		glColor3f(1, 0, 0);
		glPointSize(3);
		glBegin(GL_POINTS);
			for (float t = 1; t < 3; t+=0.1) 
			{//5 seconds, should be long enough to hit something
				if (collision(x, y, false))
				{
					break;
				}
				else
				{
					if (dy > 0)
					{
						y += (vv * t) + (0.5 * -g * t * t);
					}
					else
					{
						y -= (vv * t);
					}

					if (dx > 0)
					{
						x += vh;
					}
					else
					{
						x -= vh;
					}
					glVertex2f(x, y);
				}
			}
		glEnd();
	glPopMatrix();
}

void Player::drawArrows()
{
	for (int i = 0; i < arrows.size(); i++)
	{
		int status = collision(arrows[i].getPosX(), arrows[i].getPosY(), false);
		if (status)
		{
			if(status == 4)
			{
				shinigamies->at(shinigamiIndex).damage(arrows[i].getPower());
			}
			arrows.erase(arrows.begin() + i);
		}
		else
		{
			arrows[i].draw();
		}
	}
}

void Player::moveLeft(bool press)
{
	keyLeft = press;
}

void Player::moveRight(bool press)
{
	keyRight = press;
}

void Player::jump()
{	
	if (!jumpLock)
	{
		keyJump = true;
		velocity = 7;
	}

}

void Player::die()
{
	dead = true;
}


void Player::bowAttack()
{
	if (!keyAttack)
	{
		keyAttack = true;
	}
	else
	{
		arrows.push_back(Arrow(bowPower, posX + size/2, posY + 6 + size / 2, mouseY - windowY, mouseX - windowX, world, &arrowSprite));
		keyAttack = false;
		bowPower = 0;
	}
}

void Player::setWindowMid(int x, int y)
{
	windowX = x;
	windowY = y;
}


void Player::movement()
{

	if (keyRight && collision(posX + movementSpeed, posY, false) != 1)
	{
		posX += movementSpeed;
		//platformDisplacment += movementSpeed;
	}
	if (keyLeft && collision(posX - movementSpeed, posY, false) != 1)
	{
		posX -= movementSpeed;		
		//platformDisplacment += movementSpeed;
	}

	if (keyJump)
	{
		jumpLock = true;
		if (velocity < 0.3 || collision(posX, posY + velocity, false) == 1)
		{
			keyJump = false;
			velocity = 1;
		}
		else {
			posY += velocity;
			velocity = velocity * 0.85;
		}
	}
}



void Player::gravity()
{
	
	if (!keyJump && collision(posX, posY - velocity, true) != 1) //if not jumping and not colliding with platform.
	{
		gravityActive = true;
		jumpLock = true;
		posY -= velocity;
		if (velocity < 6)
		{
			velocity = velocity * 1.1;
		}
	}
	else if(!keyJump){ //if not jumping and is colliding with platform.
		gravityActive = false;
		jumpLock = false;
		velocity = 1;
	}

}

void Player::damage(int d)
{
	if (health <= 0) 
	{
		die();
	}
	else
	{
		//camera->setHit();
		health -= d;
	}
}

int Player::getHealth()
{
	return health;
}

int Player::getPoints()
{
	return points;
}

int Player::getBowPower()
{
	return bowPower;
}

void Player::addBowPower()
{
	if (keyAttack && bowPower < 100)
	{
		bowPower++;
	}
}

void Player::addPoints(int points)
{
	this->points += points;
}

bool Player::isVictory()
{
	return victory;
}

//Returns - No collision: 0, Platform '#': 1, Spikes '^': 2, Boosters '!': 3,
int Player::collision(GLfloat x, GLfloat y, bool player)
{
	for (int i = 0; i < world->at('#').size(); i++)
	{
		if (x + size > world->at('#')[i][0] && x < world->at('#')[i][1] && y + size > world->at('#')[i][2] && y < world->at('#')[i][3])
		{
			if (player && !keyLeft && !keyRight)
			{
				posY = world->at('#')[i][3];
			}
			return 1;
		}
	}
	for (int i = 0; i < world->at('h').size(); i++)
	{
		if (x + size > world->at('h')[i][0] && x < world->at('h')[i][1] && y + size >= world->at('h')[i][2] && y < world->at('h')[i][3])
		{

			if (player && !keyLeft && !keyRight)
			{				
				if (*platformSwitch)
				{
					posX--;
				}
				else
				{
					posX++;
				}
				posY = world->at('h')[i][3];
			}
			return 1;
		}

	}

	for (int i = 0; i < world->at('v').size(); i++)
	{
		if (x + size > world->at('v')[i][0] && x < world->at('v')[i][1] && y + size > world->at('v')[i][2] && y < world->at('v')[i][3])
		{
			if (player)
			{
				if (*platformSwitch)
				{
					posY = world->at('v')[i][3]-1;
				}
				else
				{
					posY = world->at('v')[i][3]+1;
				}

			}
			return 1;
		}
	}


	for (int i = 0; i < world->at('^').size(); i++)
	{
		if (x + size > world->at('^')[i][0] && x < world->at('^')[i][1] && y + size > world->at('^')[i][2] && y < world->at('^')[i][3])
		{
			if (player)
			{
				die();
			}
			return 2;
		}
	}

	for (int i = 0; i < world->at('!').size(); i++)
	{
		if (x + size > world->at('!')[i][0] && x < world->at('!')[i][1] && y + size > world->at('!')[i][2] && y < world->at('!')[i][3])
		{
			if (player)
			{
				keyJump = true;
				velocity = 15;
			}
			return 3;
		}
	}

	for (int i = 0; i < world->at('*').size(); i++)
	{
		if (x + size > world->at('*')[i][0] && x < world->at('*')[i][1] + 15 && y + size > world->at('*')[i][2] && y < world->at('*')[i][3] + 15)
		{
			if (player)
			{
				camera->setHit();
				damage(1);
			}
			shinigamiIndex = i;
			return 4;
		}
	}

	for (int i = 0; i < world->at('$').size(); i++)
	{
		if (x + size > world->at('$')[i][0] && x < world->at('$')[i][1] && y + size * 2> world->at('$')[i][2] && y < world->at('$')[i][3])
		{
			if (player)
			{
				addPoints(1);
				world->at('$').erase(world->at('$').begin() + i);
			}
			return 5;
		}
	}

	for (int i = 0; i < world->at('p').size(); i++)
	{
		if (x + size > world->at('p')[i][0] && x < world->at('p')[i][1] && y + size > world->at('p')[i][2] && y < world->at('p')[i][3])
		{
			if (player)
			{
				victory = true;
			}
			return 5;
		}
	}


	return false;
}