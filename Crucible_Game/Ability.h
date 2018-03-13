#ifndef ABILITY_H
#define ABILITY_H

#include "Game.h"
#include "Helper.h"
#include "GenInfo.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "Helper.h"
#include <random>
#include <vector>
#include <iostream>

class AbEffect {
public:
	enum DamageType {
		PHYS,
		FIRE,
		ICE,
		LGHT,
		POIS
	};
	struct Damage {
		DamageType type;
		int min, max;
		Damage(DamageType type, int min, int max)
		{
			this->type = type;
			this->min = min;
			this->max = max;
		}
		Damage() {}
		int getDamage() {
			std::random_device rd; 
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> dist(0, max - min);
			return dist(gen) + min;
		}
	};
	struct Effect {
		Helper::Stats stats;
		Damage damage;
		int dur;
		Effect(Helper::Stats stats, Damage damage, int dur)
		{
			this->stats = stats;
			this->damage = damage;
			this->dur = dur;
		}
		Effect() {}
	};
	Effect eff;

	virtual Effect getEffects() { return eff; }

	AbEffect(Effect eff)
	{
		this->eff = eff;
	}
	~AbEffect() {}
private:
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
	std::vector<AbEffect> effs;
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

	std::vector<AbEffect::Effect> getEffects() {
		std::vector<AbEffect::Effect> effEffs;
		for (auto eff : this->effs)
		{
			effEffs.push_back(eff.getEffects());
		}
		return effEffs;
	}

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
		this->effs = a.effs;
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
		AbEffect eff = AbEffect(AbEffect::Effect({ 0,0,0,0 },
			AbEffect::Damage(
				AbEffect::DamageType::ICE, 1, 10),
			1)); 
		AbEffect eff2 = AbEffect(AbEffect::Effect({ 0,0,0,0 },
				AbEffect::Damage(
					AbEffect::DamageType::PHYS, 50,50),
				5));
		this->effs.push_back(eff);
		this->effs.push_back(eff2);
	}

	~Ability();
private:
	Helper helper;
};

#endif /* ABILITY_H */