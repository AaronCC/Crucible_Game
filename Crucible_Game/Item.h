#include "Game.h"

#ifndef ITEM_H
#define ITEM_H

class Item
{
public:
	Item(std::string name)
	{
		this->name = name;
	}
	~Item();

	std::string name;
};

#endif /* ITEM_H */