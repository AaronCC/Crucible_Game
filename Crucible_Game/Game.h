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

	std::stack<GameState*> states;

	void pushState(GameState* state);
	void popState();
	void changeState(GameState* state);
	GameState* peekState();

	void gameLoop();

	sf::RenderWindow window;
	Game();
	~Game();

};

#endif /* GAME_H */