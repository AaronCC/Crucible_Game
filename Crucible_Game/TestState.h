#ifndef TEST_STATE_H
#define TEST_STATE_H

#include "GameState.h"
#include "Map.h"

class TestState : public GameState
{

public:
	TestState(Game* game);
	~TestState();

	virtual void draw(const float dt);
	virtual void update(const float dt);
	virtual void handleInput();

private:
	sf::Font testFont;
	sf::Text testText;

	Map* map;
};

#endif /* TEST_STATE_H */