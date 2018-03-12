#ifndef ENEMY_H
#define ENEMY_H

#include "AnimationHandler.h"
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Hud.h"
#include "Helper.h"
#include "Ability.h"
#include <stack>

class Enemy
{
public:

	Game * game;

	int hp;
	int maxHp;
	sf::Sprite sprite;
	sf::Vector2i tilePos;
	sf::Vector2f pos;
	sf::RectangleShape hpBar;
	sf::RectangleShape hpBarBack;

	Enemy(std::string spriteName, Game* game, sf::Vector2i pos, int hp)
	{
		this->game = game;
		this->hp = hp;
		this->maxHp = hp;
		this->tilePos = pos;
		this->pos = { (float)pos.x*TILE_SIZE,(float)pos.y*TILE_SIZE };
		this->sprite.setTexture(this->game->texmgr.getRef(spriteName));
		this->sprite.setOrigin(TILE_SIZE/2, TILE_SIZE/2);
		this->sprite.setPosition(this->pos);
		hpBar.setOrigin(0, 0);
		hpBar.setFillColor(sf::Color::Green);
		hpBar.setSize({ TILE_SIZE, 4 });
		hpBarBack.setOrigin(0, 0);
		hpBarBack.setFillColor(sf::Color::Red);
		hpBarBack.setSize({ TILE_SIZE, 4 });
		this->dealDamage(1);
	}

	void dealDamage(int dmg);

	virtual void update();
	virtual void draw();

	~Enemy();
};

//
//class Dummy : public Enemy
//{
//public:
//	virtual void update();
//	virtual void draw();
//	Dummy(std::string spriteName, Game* game, sf::Vector2i pos, int hp)
//		: Enemy(spriteName, game, pos)
//	{
//		this->hp = hp;
//	}
//};

#endif