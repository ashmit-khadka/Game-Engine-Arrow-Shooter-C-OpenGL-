#pragma once

#include <Windows.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
#include <vector>
#include "character.h"
#include <map>
#include <string>
#include "shinigami.h"
#include "texture.h"
#include "arrow.h"
#include "camera.h"

class Player: public Character
{

private:
	bool keyLeft, keyRight, keyJump, jumpLock, keyAttack, victory, *platformSwitch;
	int shinigamiIndex, swordRange, points, windowX, windowY, bowPower = 0, mouseX, mouseY, runSpriteIdx, frameTimer = 0;
	GLuint topIdle, topAttack, jumpSprite, fallSprite, arrowSprite;
	GLfloat movementSpeed = 2;
	std::map<char, std::vector<std::vector<int>>>* world;
	std::vector<Shinigami>* shinigamies;
	std::vector<Arrow> arrows;
	std::vector<GLuint> runSprite;
	Sprite sprite;
	Camera * camera;
	float mouseAngle = 0;

public:	
	Player() {}
	Player(int posX, int posY);
	void updateWorld(std::map<char, std::vector<std::vector<int>>>* world, std::vector<Shinigami>* shinigamies, bool* platformSwitch, Camera * camera);
	void draw(int& mouseX, int& mouseY);
	void moveLeft(bool press);
	void moveRight(bool press);
	void jump();
	void die();
	void movement();
	void gravity();
	void swordAttack();
	void bowAttack();
	void damage(int d);
	void setWindowMid(int x, int y);
	int getPoints();
	int getHealth();
	int getBowPower();
	void drawArrows();
	void addBowPower();
	bool isVictory();
	void addPoints(int points);
	void drawProjection(float power, float a, float x, float y, int dy, int dx);
	int collision(GLfloat x, GLfloat y, bool stay);

};