#pragma once
#include "player.h"
#include "shinigami.h"
#include "texture.h"
#include <fstream>
#include <map>
#include <string>
#include "camera.h"

class Game {

private:
	int gameHeight;
	int gameWidth;
	int platformDisplacement, spriteTimer = 0;
	bool platformSwitch, tutorial;
	int mouseX, mouseY;
	int windowHeight, windowWidth;
	int maxPoint;
	Camera * camera;
	GLuint coin = texture().loadPNG("textures/world/coin.png");
	GLuint patform = texture().loadPNG("textures/world/platform.png");
	GLuint background = texture().loadPNG("textures/world/hep.png");
	GLuint spikes = texture().loadPNG("textures/world/spikes.png");
	GLuint booster = texture().loadPNG("textures/world/booster.png");
	GLuint portal = texture().loadPNG("textures/world/portal.png");


	

	std::map<char, std::vector<std::vector<int>>> world =
	{
		{'#', std::vector<std::vector<int>>{}},
		{'^', std::vector<std::vector<int>>{}},
		{'!', std::vector<std::vector<int>>{}},
		{'*', std::vector<std::vector<int>>{}},
		{'h', std::vector<std::vector<int>>{}},
		{'v', std::vector<std::vector<int>>{}},
		{'$', std::vector<std::vector<int>>{}},
	};

	std::vector<Shinigami> shinigamies;
	Player player;
	GLuint txr_platform, txr_platformMoving;

	
	int mode;


public:

	friend class main;

	Game() {};

	~Game() {};

	//Constuctor for game.
	Game(
		int gameHeight,
		int gameWidth,
		Camera* camera
		)
	{
		this->gameHeight = gameHeight;
		this->gameWidth = gameWidth;
		this->camera = camera;
		readWorld("world.txt");
		platformDisplacement = 0;
		platformSwitch = false;
		mouseX = 0;
		mouseY = 0;
		txr_platform = texture().loadPNG("textures/core/morocco-blue.png");
		//txr_platformMoving = texture().loadPNG("textures/core/light-grey-terrazzo.png");
		mode = 0;
		tutorial = false;
	}

	//draws world objects.
	void draw() 
	{
		float horizontalScale, verticalScale;

		if (spriteTimer > 100)
		{
			spriteTimer = 0;
		}
		else
		{
			spriteTimer++;
		}

		//Background
		glBindTexture(GL_TEXTURE_2D, background);
		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_QUADS);
		for (int i = 0; i < world.at('#').size(); i++)
		{
			horizontalScale = gameWidth / 300;
			verticalScale = gameHeight / 300;

			glTexCoord2f(0.0, 0.0);								glVertex2f(-gameWidth, -gameHeight);
			glTexCoord2f(0.0, verticalScale);					glVertex2f(-gameWidth, gameHeight);
			glTexCoord2f(horizontalScale, verticalScale);		glVertex2f(gameWidth, gameHeight);
			glTexCoord2f(horizontalScale, 0.0);					glVertex2f(gameWidth, -gameHeight);
		}
		glEnd();
		glPopMatrix();
		glBindTexture(GL_TEXTURE_2D, 0);

		//Platforms
		glBindTexture(GL_TEXTURE_2D, patform);
		glPushMatrix();
			glColor3f(1, 1, 1);
			glBegin(GL_QUADS);
			for (int i = 0; i < world.at('#').size(); i++)
			{
				horizontalScale = (world.at('#')[i][1] - world.at('#')[i][0]) / 10;
				verticalScale = (world.at('#')[i][2] - world.at('#')[i][3]) / 10;

				glTexCoord2f(0.0, 0.0);							glVertex2f(world.at('#')[i][0], world.at('#')[i][2]);
				glTexCoord2f(0.0, verticalScale);				glVertex2f(world.at('#')[i][0], world.at('#')[i][3]);
				glTexCoord2f(horizontalScale, verticalScale);	glVertex2f(world.at('#')[i][1], world.at('#')[i][3]);
				glTexCoord2f(horizontalScale, 0.0);				glVertex2f(world.at('#')[i][1], world.at('#')[i][2]);
			}
			glEnd();
		glPopMatrix();

		//Vertical Platforms
		glPushMatrix();
			glColor3f(1.0, 1.0, 1.0);
			glBegin(GL_QUADS);
			for (int i = 0; i < world.at('v').size(); i++)
			{
				horizontalScale = (world.at('v')[i][1] - world.at('v')[i][0]) / 10;
				verticalScale = (world.at('v')[i][2] - world.at('v')[i][3]) / 10;

				if (platformSwitch)
				{
					world.at('v')[i][2] --;
					world.at('v')[i][3] --;
				}
				else
				{
					world.at('v')[i][2] ++;
					world.at('v')[i][3] ++;
				}

				glTexCoord2f(0.0, 0.0);							glVertex2f(world.at('v')[i][0], world.at('v')[i][2]);
				glTexCoord2f(0.0, verticalScale);				glVertex2f(world.at('v')[i][0], world.at('v')[i][3]);
				glTexCoord2f(horizontalScale, verticalScale);	glVertex2f(world.at('v')[i][1], world.at('v')[i][3]);
				glTexCoord2f(horizontalScale, 0.0);				glVertex2f(world.at('v')[i][1], world.at('v')[i][2]);
			}
			glEnd();
		glPopMatrix();

		//Horizontal Platforms
		glPushMatrix();
			glColor3f(1.0, 1.0, 1.0);
			glBegin(GL_QUADS);
			for (int i = 0; i < world.at('h').size(); i++)
			{
				horizontalScale = (world.at('h')[i][1] - world.at('h')[i][0]) / 10;
				verticalScale = (world.at('h')[i][2] - world.at('h')[i][3]) / 10;

				if (platformSwitch)
				{
					world.at('h')[i][0] --;
					world.at('h')[i][1] --;
				}
				else
				{
					world.at('h')[i][0] ++;
					world.at('h')[i][1] ++;
				}

				glTexCoord2f(0.0, 0.0);							glVertex2f(world.at('h')[i][0], world.at('h')[i][2]);
				glTexCoord2f(0.0, verticalScale);				glVertex2f(world.at('h')[i][0], world.at('h')[i][3]);
				glTexCoord2f(horizontalScale, verticalScale);	glVertex2f(world.at('h')[i][1], world.at('h')[i][3]);
				glTexCoord2f(horizontalScale, 0.0);				glVertex2f(world.at('h')[i][1], world.at('h')[i][2]);
			}
			glEnd();
		glPopMatrix();
		glBindTexture(GL_TEXTURE_2D, 0);

		//switch moving platform directions
		if (platformSwitch)
		{
			platformDisplacement--;
		}
		else
		{
			platformDisplacement++;
		}

		if (platformDisplacement > 50)
		{
			platformSwitch = true;
		}
		if (platformDisplacement < 0)
		{
			platformSwitch = false;
		}


		//spikes
		glBindTexture(GL_TEXTURE_2D, spikes);
		glPushMatrix();
			glColor3f(1.0, 1.0, 1.0);
			glBegin(GL_QUADS);
			for (int i = 0; i < world.at('^').size(); i++)
			{
				horizontalScale = (world.at('^')[i][1] - world.at('^')[i][0]) / 10;
				verticalScale = (world.at('^')[i][2] - world.at('^')[i][3]) / 10;

				glTexCoord2f(0.0, 0.0);							glVertex2f(world.at('^')[i][0], world.at('^')[i][2]);
				glTexCoord2f(0.0, -verticalScale);				glVertex2f(world.at('^')[i][0], world.at('^')[i][3]);
				glTexCoord2f(horizontalScale, -verticalScale);	glVertex2f(world.at('^')[i][1], world.at('^')[i][3]);
				glTexCoord2f(horizontalScale, 0.0);				glVertex2f(world.at('^')[i][1], world.at('^')[i][2]);
			}
			glEnd();
		glPopMatrix();
		glBindTexture(GL_TEXTURE_2D, 0);


		//boosters
		glBindTexture(GL_TEXTURE_2D, booster);
		glPushMatrix();
			glColor3f(1.0, 1.0, 1.0);
			glBegin(GL_QUADS);
			for (int i = 0; i < world.at('!').size(); i++)
			{
				glTexCoord2f(0.0, 0.0);							glVertex2f(world.at('!')[i][0], world.at('!')[i][2]);
				glTexCoord2f(0.0, 1.0);				glVertex2f(world.at('!')[i][0], world.at('!')[i][3]);
				glTexCoord2f(1.0, 1.0);	glVertex2f(world.at('!')[i][1], world.at('!')[i][3]);
				glTexCoord2f(1.0, 0.0);				glVertex2f(world.at('!')[i][1], world.at('!')[i][2]);
			}
			glEnd();
		glPopMatrix();
		glBindTexture(GL_TEXTURE_2D, 0);


		//coins
		glBindTexture(GL_TEXTURE_2D, coin);
		glPushMatrix();
			glColor3f(1.0, 1.0, 1.0);
			glBegin(GL_QUADS);
				for (int i = 0; i < world.at('$').size(); i++)
				{
					glTexCoord2f(0.0, 0.0); glVertex2f(world.at('$')[i][0], world.at('$')[i][2]);
					glTexCoord2f(0.0, 1.0); glVertex2f(world.at('$')[i][0], world.at('$')[i][3]);
					glTexCoord2f(1.0, 1.0); glVertex2f(world.at('$')[i][1], world.at('$')[i][3]);
					glTexCoord2f(1.0, 0.0); glVertex2f(world.at('$')[i][1], world.at('$')[i][2]);
				}
			glEnd();
		glPopMatrix();
		glBindTexture(GL_TEXTURE_2D, 0);

		//objective
		glBindTexture(GL_TEXTURE_2D, portal);
		glPushMatrix();
			glColor3f(1.0, 1.0, 1.0);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0, 0.0); glVertex2f(world.at('p')[0][0], world.at('p')[0][2]);
				glTexCoord2f(0.0, 1.0); glVertex2f(world.at('p')[0][0], world.at('p')[0][3]);
				glTexCoord2f(1.0, 1.0); glVertex2f(world.at('p')[0][1], world.at('p')[0][3]);
				glTexCoord2f(1.0, 0.0); glVertex2f(world.at('p')[0][1], world.at('p')[0][2]);
		glEnd();
		glPopMatrix();
		glBindTexture(GL_TEXTURE_2D, 0);

		//Shinigami

		for (int i = 0; i < shinigamies.size(); i++)
		{
			if(shinigamies[i].isDead() ==true)
			{
				shinigamies.erase(shinigamies.begin() + i);
				player.addPoints(20);
			}
			else
			{
				shinigamies[i].draw(&world);
				shinigamies[i].updateWorld(player.getPosX(), player.getPosY());
			}

		}

		if (tutorial)
		{
			drawTutorial();
		}
		drawHub(-80, -80, 10);
		updateWorld();

		if (!player.isDead())
		{
			player.updateWorld(&world, &shinigamies, &platformSwitch, camera); //no need to update pointer.
			player.draw(mouseX, mouseY);
		}
		else
		{
			mode = 2;
			getTopScore();
			tutorial = false;
		}
		if (player.isVictory())
		{
			mode = 3;
			getTopScore();
			tutorial = false;
		}
	}

	void drawTutorial()
	{
		std::string stats;
		char buffer[200];
		glPushMatrix();
			glColor4f(0.0, 0.0, 0.0, 0.7);
			glBegin(GL_QUADS);
				glVertex2f(player.getPosX() + 20, player.getPosY() + 40);
				glVertex2f(player.getPosX() + 20, player.getPosY() + 90);
				glVertex2f(player.getPosX() + 90, player.getPosY() + 90);
				glVertex2f(player.getPosX() + 90, player.getPosY() + 40);
			glEnd();
		glPopMatrix();
		glColor3f(1, 1, 1);
		glRasterPos2f(player.getPosX() + 25, player.getPosY() + 85);
		stats = sprintf_s(buffer, "Controls:\nA - Move left\nD - Move Right\nW - Jump\nMouse - Aim/Shoot\n\n"
			"Objective:\nFind the portal and\nescape the forest.\n\nPress F to toggle tutorial.");
		glutBitmapString(GLUT_BITMAP_9_BY_15, (const unsigned char*)buffer);
	}

	//draw the hame hub
	void drawHub(int posX, int posY, int radius) 
	{
		std::string stats;
		char buffer[100];

		glPushMatrix();
			glColor4f(0.0, 0.0, 0.0, 0.7);
			glBegin(GL_QUADS);
				glVertex2f(player.getPosX() + posX, player.getPosY() + posY);
				glVertex2f(player.getPosX() + posX, player.getPosY() + posY + 25);
				glVertex2f(player.getPosX() + posX + 35, player.getPosY() + posY + 25);
				glVertex2f(player.getPosX() + posX + 35, player.getPosY() + posY);
			glEnd();
		glPopMatrix();

		glColor3f(1, 1, 1);
		glRasterPos2f(player.getPosX() + posX + 5, player.getPosY() + posY + 15);
		stats = sprintf_s(buffer, "Health: %d\nPoints: %d\nPower:  %d", player.getHealth(), player.getPoints(), player.getBowPower());
		glutBitmapString(GLUT_BITMAP_9_BY_15, (const unsigned char*)buffer);
	}

	//reads world from world.txt.
	void readWorld(const std::string &path)
	{
		int worldHeight = 1000, worldWidth = 1000, tileSize = 10, Xcnt = 0, Ycnt = 0, Xmin = 0, Xmax = 0, Ymin = 0, Ymax = 0, coinRadius = 2;
		std::ifstream reader(path);     // open file
		int lastTile;
		char c;
		while (reader.get(c)) 
		{
			//printf("Xcnt: %d, Ycnt: %d\n", Xcnt, Ycnt);

			if (c == '#' || c == '^' || c == '!' || c == '*' || c == 'h' || c == 'v' || c=='$' || c == '@' || c == 'p') {
				//printf("Xmin: %d, Xmax: %d, Ymin: %d, Ymax: %d\n", Xmin, Xmax, Ymin, Ymax);
				Xmin = -(worldWidth / 2) + (Xcnt * tileSize);
				Xmax = Xmin + tileSize;
				Ymax = worldHeight / 2 - (Ycnt * tileSize);
				Ymin = Ymax - tileSize;
				if (c == '@')
				{
					player = Player(Xmin, Ymin);
				}

				if (c == '*')
				{
					world['*'].push_back(std::vector<int>{Xmin, Xmax, Ymin, Ymax});
					shinigamies.push_back(Shinigami(Xmin, Ymin));
				}
				if (c == '$')
				{
					world['$'].push_back(std::vector<int>
					{
						Xmin + (tileSize/2) - coinRadius,
						Xmax - (tileSize / 2) + coinRadius,
						Ymin + (tileSize / 2) - coinRadius,
						Ymax - (tileSize / 2) + coinRadius
					});
				}
				else
				{
					if (lastTile == c)
					{
						world[c].back()[1] = Xmax;
					}
					else
					{
						world[c].push_back(std::vector<int>{Xmin, Xmax, Ymin, Ymax});
					}
				}
			}
			if (Xcnt == 100) {
				Xcnt = 0;
				Ycnt++;
			}
			else {
				Xcnt++;
			}
			lastTile = c;
		}
		reader.close();
	}

	void updateWorld()
	{
		world['*'].clear();
		for (Shinigami s : shinigamies)
		{
			world['*'].push_back(std::vector<int>{(int)s.getPosX(), (int)s.getPosX()+10, (int)s.getPosY(), (int)s.getPosY() + 10}); //+10 should be global
		}
	}

	void setMouseCoordinates(const int &x, const int &y)
	{
		mouseX = x;
		mouseY = y;
		//printf("x: %d, y: %d\n", mouseX, mouseY);
	}

	void setWindowDimension(const int& w, const int& h)
	{
		windowWidth = w;
		windowHeight = h;
		player.setWindowMid(w/2 + player.getSize()/2, h/2 + player.getSize() / 2);
	}

	int getGameHeight()
	{
		return gameHeight;
	}

	int getGameWidth()
	{
		return gameWidth;
	}

	Player& getPlayer() {
		return player;
	}

	void setMode(int mode)
	{
		this->mode = mode;
	}

	int getMode()
	{
		return mode;
	}

	void toggleTutorial()
	{
		tutorial = (tutorial == true) ? false : true;
	}

	int getTopScore()
	{
		std::ifstream reader("points.txt");     // open file

		if (reader.good())
		{
			std::string sLine;
			getline(reader, sLine);
			maxPoint = std::stoi(sLine);

			if (maxPoint < player.getPoints())
			{
				maxPoint = player.getPoints();
				printf("yee\n");
				std::ofstream writer("points.txt", std::ofstream::out);
				writer << maxPoint;
				writer.close();
			}
		}
		reader.close();
		return maxPoint;
	}



	
	void reset()
	{
		world =
		{
			{'#', std::vector<std::vector<int>>{}},
			{'^', std::vector<std::vector<int>>{}},
			{'!', std::vector<std::vector<int>>{}},
			{'*', std::vector<std::vector<int>>{}},
			{'h', std::vector<std::vector<int>>{}},
			{'v', std::vector<std::vector<int>>{}},
			{'$', std::vector<std::vector<int>>{}},
		};
		shinigamies.clear();
		platformDisplacement = 0;
		platformSwitch = false;
		mouseX = 0;
		mouseY = 0;
		player = Player();
		readWorld("world.txt");
		glutReshapeWindow(800, 800);
	}
};