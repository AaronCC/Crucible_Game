#ifndef EXPLORE_STATE_H
#define EXPLORE_STATE_H

#include "GameState.h"
#include "Player.h"
#include "Map.h"
#include "Camera.h"
#include "PathFinder.h"

class ExploreState : public GameState
{
public:
	ExploreState(Game* game);
	~ExploreState();

	Camera camera;
	Player player;
	int fps;
	int fTotal;
	float fTime;
	std::pair<bool,int> resolving;
	float rTimeTotal = 0;
	float rTime;

	std::vector<Tile*> fowCache;

	std::vector<Ability*> activeAbilities;

	virtual void draw(const float dt);
	virtual void update(const float dt);
	virtual void handleInput();
	void resolveFoW();
	void resolveGameState(unsigned int ticks);

private:
	bool old_mLeftState = false;
	bool old_wState = false;
	bool old_sState = false;
	bool old_aState = false;
	bool old_dState = false;
	sf::Font testFont;
	sf::Text testText;
	PathFinder pf;
	Map* map;
};

#endif /* EXPLORE_STATE_H */