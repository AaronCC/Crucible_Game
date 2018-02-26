#ifndef ABILITY_H
#define ABILITY_H

#include "Game.h"
#include "Helper.h"
#include "GenInfo.h"

class AbEffect {
public:
	enum EffType {
		DEBUFF,
		BUFF,
		INST
	};
	EffType effType;

	AbEffect()
	{

	}
	~AbEffect() {}
private:
};

class DebuffEff : public AbEffect {

};
class BuffEff : public AbEffect {

};
class InstEff : public AbEffect {

};
class Ability
{
public:
	enum ID {
		SLASH
	};
	enum AbPrmType {
		RANGED,
		MELEE
	};
	enum AbSecType {
		AREA,
		TARG
	};
	AbSecType secType;

	struct AbInfo {
		AbPrmType prm;
		AbSecType sec;
		std::vector<AbEffect> eff;
		int area;
	};
	AbInfo info;
	std::vector<sf::Vector2f> drawPositions;
	std::vector<sf::Vector2i> area;

	std::vector<sf::Vector2i> getArea(sf::Vector2i origin);

	std::vector<sf::Vector2i> getActiveTiles(sf::Vector2i pPos, sf::Vector2i mPos);


	void pushPosition(sf::Vector2f pos);

	sf::Sprite sprite;
	AnimationHandler animHandler;
	Game* game;

	int range;

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
		this->info = a.info;
		this->secType = a.secType;
		this->range = a.range;
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
		this->animHandler.frameSize = sf::IntRect(0, 0, size.x, size.y);
		this->animHandler.addAnim(animation);
		this->duration = animation.duration * animation.getLength();
		this->description = description;
		this->info.area = 1;
		this->secType = AbSecType::AREA;
		this->range = 4;
	}

	~Ability();
private:
	Helper helper;
};

#endif /* ABILITY_H */