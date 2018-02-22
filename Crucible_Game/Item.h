#include "Game.h"

#ifndef ITEM_H
#define ITEM_H

class Item
{
public:
	enum SlotType {
		HED = 0,
		BDY = 1,
		MAH = 2,
		OHH = 3,
		RNG = 4,
		AMU = 5,
		CLK = 6,
		BLT = 7
	};
	SlotType slotType;

	struct Buffs {
		int attack;
		int defense;
		int agility;
		int knowledge;
	};

	Buffs buffs;
	std::vector<std::string> getBuffString()
	{
		std::vector<std::string> buffStr;
		if (buffs.attack > 0)
			buffStr.push_back("Attack +" + std::to_string(buffs.attack));
		if (buffs.defense > 0)
			buffStr.push_back("Defense +" + std::to_string(buffs.defense));
		if (buffs.agility > 0)
			buffStr.push_back("Agility +" + std::to_string(buffs.agility));
		if (buffs.knowledge > 0)
			buffStr.push_back("Knowledge +" + std::to_string(buffs.knowledge));
		return buffStr;
	}

	Item(std::string name, Buffs buffs, SlotType type)
	{
		this->buffs = buffs;
		this->name = name;
		this->slotType = type;
	}
	~Item();

	std::string name;

	std::vector<std::string> propertyText;
};

#endif /* ITEM_H */