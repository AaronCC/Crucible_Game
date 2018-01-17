#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <stack>

class GameState;

class Game
{
public: 

	const float aspectRatio = 800 / (float)600;

	Game();
	~Game();
};

#endif /* GAME_H */