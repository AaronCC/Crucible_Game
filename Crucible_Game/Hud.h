#ifndef HUD_H
#define HUD_H
#include "Game.h"

class Hud
{
public:
	std::map<std::string, sf::Sprite> elements;
	Game* game;
	Hud() {};
	Hud(Game * game, std::vector<std::string> eData)
	{
		this->game = game;
		/* Health */
		elements["hGlobe"].setTexture(game->texmgr.getRef(eData[0]));
		elements["hGlobe"].setPosition(15.f, game->windowSize.y - 15.f);
		elements["hGlobe"].setOrigin(0, elements["hGlobe"].getTexture()->getSize().y);

		elements["hPool"].setTexture(game->texmgr.getRef(eData[1]));
		elements["hPool"].setPosition(15.f, game->windowSize.y - 15.f);
		elements["hPool"].setOrigin(0, elements["hPool"].getTexture()->getSize().y);
	}

	void draw(float dt);
	void update(float dt);

	void updateHealth(float dmg);

	~Hud();
};

#endif /* HUD_H */