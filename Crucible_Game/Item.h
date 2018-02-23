#include "Game.h"
#include "Ability.h"

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
	enum Buffs {
		attack = 0,
		defense = 1,
		agility = 2,
		knowledge = 3
	};

	std::vector<std::string> buffStrings{ "Attack","Defense","Agility","Knowledge" };
	std::vector<std::string> typeStrings{ "Head","Body","Main-hand","Off-hand","Ring","Amulet","Cloak","Belt","Scroll" };
	std::map<int, int> buffs;

	bool twoHanded = false;

	std::string getTypeString()
	{
		return typeStrings[slotType];
	}

	std::vector<std::string> getBuffString()
	{
		std::vector<std::string> buffStr;
		for (int i = 0; i < NUM_ITEM_BUFFS; i++)
		{
			int val = buffs[i];
			if(val > 0)
				buffStr.push_back(buffStrings[i] + " +" + std::to_string(val));
			else if(val < 0)
				buffStr.push_back(buffStrings[i] + " " + std::to_string(val));
		}
		return buffStr;
	}

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
		: Item(name,buffs,type) {
		this->ability = ability;
	}
	std::string getDesc() {
		return ability->description;
	}
private:
	Ability * ability;
};
#endif /* ITEM_H */