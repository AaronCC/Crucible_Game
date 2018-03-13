#include "Game.h"
#include "Ability.h"
#include "GenInfo.h"

#ifndef ITEM_H
#define ITEM_H

class Item
{
public:
	enum SlotType {
		HED = 0,
		BDY = 1,
		MAH = 2,
		OFH = 3,
		RNG = 4,
		AMU = 5,
		CLK = 6,
		BLT = 7,
		SCR = 8
	};
	SlotType slotType;
#define NUM_ITEM_BUFFS 4

	std::vector<std::string> buffStrings{ "Attack","Defense","Agility","Knowledge" };
	std::vector<std::string> typeStrings{ "Head","Body","Main-hand","Off-hand","Ring","Amulet","Cloak","Belt","Scroll" };
	std::map<int, int> buffs;

	bool twoHanded = false;

	std::string getTypeString()
	{
		return typeStrings[slotType];
	}
	virtual std::string getName()
	{
		return this->name;
	}
	virtual std::vector<std::pair<sf::Color, std::string>> getBuffString()
	{
		std::vector<std::pair<sf::Color, std::string>> buffStr;
		for (int i = 0; i < NUM_ITEM_BUFFS; i++)
		{
			int val = buffs[i];
			if (val > 0)
				buffStr.push_back({ sf::Color::White, buffStrings[i] + " +" + std::to_string(val) });
			else if (val < 0)
				buffStr.push_back({ sf::Color::White, buffStrings[i] + " " + std::to_string(val) });
		}
		return buffStr;
	}
	virtual std::string getItemTexName() { return ""; }

	Item(std::string name, std::vector<int> buffs, SlotType type)
	{
		this->name = name;
		this->slotType = type;
		for (int i = 0; i < NUM_ITEM_BUFFS; i++)
		{
			this->buffs[i] = buffs[i];
		}
	}
	~Item();

	std::string name;

	std::vector<std::string> propertyText;
};

class Scroll : public Item {
public:
	Scroll(std::string name, std::vector<int> buffs, SlotType type, Ability* ability)
		: Item(name, buffs, type) {
		this->ability = ability;
	}
	std::string getDesc() {
		return ability->description;
	}

	std::vector<std::pair<sf::Color, std::string>> getBuffString()
	{
		std::map<AbEffect::DamageType, std::string> damageStrings;
		damageStrings[AbEffect::DamageType::FIRE] = "Fire";
		damageStrings[AbEffect::DamageType::ICE] = "Ice";
		damageStrings[AbEffect::DamageType::LGHT] = "Lightning";
		damageStrings[AbEffect::DamageType::PHYS] = "Physical";
		damageStrings[AbEffect::DamageType::POIS] = "Poison";
		std::map<AbEffect::DamageType, sf::Color> damageColors;
		damageColors[AbEffect::DamageType::FIRE] = sf::Color({ 255, 74, 61 });
		damageColors[AbEffect::DamageType::ICE] = sf::Color({ 109, 165, 255 });
		damageColors[AbEffect::DamageType::LGHT] = sf::Color({ 248, 255, 61 });
		damageColors[AbEffect::DamageType::PHYS] = sf::Color({ 226, 165, 86 });
		damageColors[AbEffect::DamageType::POIS] = sf::Color({ 131, 211, 69 });

		std::vector<std::pair<sf::Color, std::string>> buffStr;

		buffStr.push_back({ sf::Color::White, getDesc() });
		std::vector<AbEffect::Effect> effs = ability->getEffects();
		for (auto eff : effs)
		{
			std::string min = std::to_string(eff.damage.min);
			std::string max = std::to_string(eff.damage.max);
			std::string type = damageStrings[eff.damage.type];
			if (eff.dur == 1)
			{
				if (min != max)
					buffStr.push_back({ damageColors[eff.damage.type], min + "-" + max + " " + type + " damage" });
				else
					buffStr.push_back({ damageColors[eff.damage.type], min + " " + type + " damage" });
			}
			else
			{
				if (min != max)
					buffStr.push_back({ damageColors[eff.damage.type], min + "-" + max + " " + type + " damage" + " over " + std::to_string(eff.dur) + " ticks" });
				else
					buffStr.push_back({ damageColors[eff.damage.type], min + " " + type + " damage" + " over " + std::to_string(eff.dur) + " ticks" });
			}
		}
		return buffStr;
	}

	virtual std::string getItemTexName() {
		return ability->texName;
	}

	virtual std::string getName() {
		return this->ability->name;
	}
private:
	Ability * ability;
};
#endif /* ITEM_H */