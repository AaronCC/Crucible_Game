#ifndef ABILITY_H
#define ABILITY_H

#include "Game.h"

class Ability
{
public:
	sf::Sprite sprite;
	AnimationHandler animHandler;
	Game* game;

	std::string id;
	sf::Vector2u size;

	float duration;
	float durTimer;
	bool isAlive;
	float cooldown;

	void activate(sf::Vector2f pPos, sf::Vector2i mPos, std::string id);
	void update(float dt);
	void draw(float dt);
	
	Ability() {}

	Ability(const Ability& a)
	{
		this->isAlive = false;
		this->game = a.game;
		this->id = a.id;
		this->sprite = a.sprite;
		this->animHandler = a.animHandler;
		this->duration = a.duration;
		this->cooldown = a.cooldown;
	}

	Ability(Game* game,
		sf::Texture& texture,
		Animation animation,
		sf::Vector2u size,
		std::string id,
		float cooldown)
	{
		this->cooldown = cooldown;
		this->size = size;
		this->id = id;
		this->isAlive = false;
		this->game = game;
		this->sprite.setTexture(texture);
		this->sprite.setOrigin(size.x / 2, size.y / 2);
		this->animHandler.changeAnim(0);
		this->animHandler.frameSize = sf::IntRect( 0,0,size.x,size.y );
		this->animHandler.addAnim(animation);
		this->duration = animation.duration * animation.getLength();
	}

	~Ability();
};

#endif /* ABILITY_H */