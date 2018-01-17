#include <SFML/Graphics.hpp>

#include "Game.h"
#include "TestState.h"

int main()
{
	Game game;

	game.pushState(new TestState(&game));
	game.gameLoop();

	return 0;
}