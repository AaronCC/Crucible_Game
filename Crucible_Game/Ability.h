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
public:
	DebuffEff() {
		this->effType = DEBUFF;
	}
};
class BuffEff : public AbEffect {
public:
	BuffEff() {
		this->effType = BUFF;
	}
};
class InstEff : public AbEffect {
public:
	InstEff() {
		this->effType = INST;
	}
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
	struct AbInfo {
		AbPrmType prm;
		AbSecType sec;
		std::vector<AbEffect> eff;
		int area;
		int range;
		AbInfo(AbPrmType prm, AbSecType sec, int area, int range)
		{
			this->prm = prm;
			this->sec = sec;
			this->area = area;
			this->range = range;
		}
		AbInfo() {}
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

	void setInfo(AbInfo info) { this->info = info; }
	void addEff(AbEffect eff) { this->info.eff.push_back(eff); }

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
		this->description = a.description;
		this->info = a.info;
		this->texName = a.texName;
	}
	std::string texName;
	Ability(Game* game,
		sf::Texture& texture,
		std::string texName,
		Animation animation,
		sf::Vector2u size,
		ID id,
		float cooldown,
		unsigned int tickCost,
		std::string name,
		std::string description)
	{
		this->name = name;
		this->texName = texName;
		this->tickCost = tickCost;
		this->slotNum = slotNum;
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
	}

	~Ability();
private:
	Helper helper;
};

#endif /* ABILITY_H */