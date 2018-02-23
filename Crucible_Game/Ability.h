#ifndef ABILITY_H
#define ABILITY_H

#include "Game.h"

class Ability
{
public:
	enum ID {
		SLASH
	};

	sf::Sprite sprite;
	AnimationHandler animHandler;
	Game* game;

	ID id;
	sf::Vector2u size;

	float duration;
	float durTimer;
	bool isAlive;
	float cooldown;
	unsigned int slotNum;
	unsigned int tickCost;
	std::string name;
	std::string description;
	int speed;

	void activate(sf::Vector2i pPos, sf::Vector2i mPos);
	std::vector<sf::Vector2i> getActiveTiles(sf::Vector2i pPos, sf::Vector2i mPos);
	void update(float dt);
	void draw(float dt);
	
	void resolveCollision() {}

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
		this->slotNum = a.slotNum;
		this->tickCost = a.tickCost;
		this->name = a.name;
		this->description = description;
	}

	Ability(Game* game,
		sf::Texture& texture,
		Animation animation,
		sf::Vector2u size,
		ID id,
		float cooldown,
		int speed,
		unsigned int slotNum,
		unsigned int tickCost,
		std::string name,
		std::string description)
	{
		this->name = name;
		this->tickCost = tickCost;
		this->slotNum = slotNum;
		this->speed = speed;
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
		this->description = description;
	}

	~Ability();
};

class Projectile : public Ability {

};

#endif /* ABILITY_H */