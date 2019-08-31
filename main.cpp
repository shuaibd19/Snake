#include "SnakeGame.h"

using namespace std;

int main()
{
	SnakeGame game;
	game.ConstructConsole(40, 28, 24, 24);
	game.Start();
}
