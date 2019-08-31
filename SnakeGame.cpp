#include "SnakeGame.h"

bool SnakeGame::OnUserCreate()
{
	snake = { {20.0f, 10.0f}, {19.0f, 10.0f}, {18.0f, 10.0f} };
	srand((unsigned int)time(NULL));
	apples.push_back({ 10.0f, 10.0f });
	turnips.push_back({ 1.0f, 1.0f });
	deathWalls.push_back({ float(rand() % 40), float(rand() % 24) });
	deathWalls.push_back({ deathWalls.front().x + 1, deathWalls.front().y });
	deathWalls.push_back({ deathWalls.front().x + 2, deathWalls.front().y });

	return true;
}

bool SnakeGame::OnUserUpdate(float fElapsedTime)
{
	if (!snakeDead)
	{
		this_thread::sleep_for(100ms);
		RenderWorld();
		snakeMovement();
		wrapAround();
		fruitCollision();
		snakeCollision();
		deathWallCollision();
	}
	else
	{
		Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, BG_BLACK);
		DrawString(ScreenWidth() / 2.5, ScreenHeight() / 2.5, L"YOU SUCK", FG_RED);
		DrawString(ScreenWidth() / 3, (ScreenHeight() / 2.5) + 2 , L"Final Score: " + to_wstring(playerScore), FG_RED);
		DrawString(ScreenWidth() / 3, (ScreenHeight() / 2.5) + 4, L"Hold Space To Play Again", FG_RED);
		_isSpaceHeld = (m_keys[VK_SPACE].bHeld || m_keys[VK_SPACE].bPressed);
		if (_isSpaceHeld)
		{
			bool resetGame = true;
			snakeDead = false;
			playerScore = 0;
			snake.clear();
			apples.clear();
			turnips.clear();
			deathWalls.clear();
			resetGame = OnUserCreate();
			this_thread::sleep_for(100ms);
			reset();
		}
	}

	return true;
}

void SnakeGame::RenderWorld()
{
	//Clear the screen by drawing the defined ground colour
	Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, GROUND_COLOUR);
	for (auto s : snake)
	{
		Draw((int)round(s.x), (int)round(s.y), 0x00A9, bodColour);
	}
	Draw(snake.front().x, snake.front().y, 0x0152, headColour);
	for (auto a : apples)
	{
		Draw((int)round(a.x), (int)round(a.y), 0x00D3, appleColour);
	}
	if (playerScore > 90)
	{
		for (auto t : turnips)
		{
			Draw((int)round(t.x), (int)round(t.y), 0x01FE, turnipColour);
		}
	}
	if (playerScore > 190)
	{
		for (auto w : deathWalls)
		{
			Draw((int)round(w.x), (int)round(w.y), 0x0058, wallColour);
		}
	}
	bool wallPlaced = false;
	DrawString(15, 0, L"Score:" + to_wstring(playerScore), FG_YELLOW);
}


void SnakeGame::snakeMovement()
{
	//get input
	_isUpKeyHeld = (m_keys[VK_UP].bHeld || m_keys[VK_UP].bPressed);
	_isDownKeyHeld = (m_keys[VK_DOWN].bHeld || m_keys[VK_DOWN].bPressed);
	_isLeftKeyheld = (m_keys[VK_LEFT].bHeld || m_keys[VK_LEFT].bPressed);
	_isRightKeyheld = (m_keys[VK_RIGHT].bHeld || m_keys[VK_RIGHT].bPressed);

	


	if (_isUpKeyHeld)
	{
		snakeDirection = 0;
	}
	else if (_isRightKeyheld)
	{
		snakeDirection = 1;
	}
	else if (_isDownKeyHeld)
	{
		snakeDirection = 2;
	}
	else if (_isLeftKeyheld)
	{
		snakeDirection = 3;
	}

	switch (snakeDirection)
	{
	case 0:
		snake.push_front({ snake.front().x, snake.front().y - 1 });
		break;
	case 1:
		snake.push_front({ snake.front().x + 1, snake.front().y });
		break;
	case 2: 
		snake.push_front({ snake.front().x, snake.front().y + 1 });
		break;
	case 3:
		snake.push_front({ snake.front().x - 1, snake.front().y });
		break;
	}

	snake.pop_back();
	
}

void SnakeGame::wrapAround()
{
	if (round(snake.front().x) >= ScreenWidth())
	{
		snake.front().x = 0;
	}

	if (round(snake.front().x) < 0)
	{
		snake.front().x = ScreenWidth();
	}

	if (round(snake.front().y) == -1)
	{
		snake.front().y = ScreenHeight();
	}

	else if (round(snake.front().y) == ScreenHeight() + 1)
	{
		snake.front().y = 0;
	}
}

void SnakeGame::fruitCollision()
{
	if (snake.front().x == apples.front().x && snake.front().y == apples.front().y)
	{
		playerScore += apples.front().score;
		apples.pop_back();
		snake.push_back({ snake.back().x, snake.back().y});
		apples.push_back({ float(rand() % 40), float(rand() % 24) });
	}

	if (playerScore>90 && snake.front().x == turnips.front().x && snake.front().y == turnips.front().y)
	{
		playerScore += turnips.front().score;
		turnips.pop_back();
		for (int i = 0; i < 3; i++)
		{
			snake.push_back({ snake.back().x, snake.back().y });
		}
		turnips.push_back({ float(rand() % 40), float(rand() % 24) });
	}
}

void SnakeGame::snakeCollision()
{
	list<Snakebits>::iterator i;
	for (i = snake.begin(); i != snake.end(); i++)
	{
		if (i != snake.begin() && i->x == snake.front().x && i->y == snake.front().y)
		{
			snakeDead = true;
		}
	}
}

void SnakeGame::reset()
{
	RenderWorld();
	snakeMovement();
	wrapAround();
	fruitCollision();
	snakeCollision();
	deathWallCollision();
}

void SnakeGame::deathWallCollision()
{
	vector<Wall>::iterator i;
	for (i = deathWalls.begin(); i != deathWalls.end(); i++)
	{
		if (playerScore > 190 && i->x == snake.front().x && i->y == snake.front().y)
		{
			snakeDead = true;
		}
	}
}


SnakeGame::SnakeGame()
{
}

SnakeGame::~SnakeGame()
{
}
