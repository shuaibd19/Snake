#pragma once
#include "olcConsoleGameEngineOOP.h"
#include <math.h>
#include <vector>
#include <iostream>
#include <time.h>
#include <list>
#include <fstream>
using namespace std;

struct Snakebits
{
	float x;
	float y;
	int speed = 5;
};

struct Wall
{
	float x;
	float y;
};

struct SuperFruit
{
	float x;
	float y;
	int score = 20;
};

struct Fruit
{
	float x;
	float y;
	int score = 10;
};


class SnakeGame : public olcConsoleGameEngineOOP
{

public:
	SnakeGame();
	~SnakeGame();
protected:
	int playerScore = 0;
	virtual bool OnUserCreate();
	virtual bool OnUserUpdate(float fElapsedTime);
	int snakeDirection = 1;

	bool _isUpKeyHeld = false;
	bool _isDownKeyHeld = false;
	bool _isLeftKeyheld = false;
	bool _isRightKeyheld = false;
	bool _isSpaceHeld = false;
	bool _isHard = false;
	bool _isCtrlHeld = false;
	bool _isEnterHeld = false;

	const int bodColour = FG_WHITE;
	const int headColour = FG_BLUE;
	const int appleColour = FG_GREEN;
	const int turnipColour = FG_DARK_YELLOW;
	const int wallColour = FG_DARK_RED;
	int highestScore = 0;

	vector<Fruit> apples;
	vector<SuperFruit> turnips;
	vector<Wall> deathWalls;
	list<Snakebits> snake;

	//the background of the screen has the foreground colour of magenta
	const int GROUND_COLOUR = FG_MAGENTA;
	bool snakeDead = false;

	void RenderWorld();
	/*Used for the movement of the snake head and the body parts that follow it*/
	void snakeMovement();
	void wrapAround();
	void fruitCollision();
	void snakeCollision();
	void reset();
	void deathWallCollision();

private:

};