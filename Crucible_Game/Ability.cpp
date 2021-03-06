#include "Ability.h"

Ability::~Ability()
{
}

void Ability::activate(sf::Vector2i pPos, sf::Vector2i mPos)
{
	this->isAlive = true;
	this->durTimer = this->duration;
	this->animHandler.changeAnim(0);
	this->animHandler.reset();
}

std::vector<sf::Vector2i> Ability::getArea(sf::Vector2i origin)
{
	if (info.sec == AbSecType::TARG)
		return { origin };
	else if (info.sec == AbSecType::AREA)
	{
		std::vector<sf::Vector2i> area;
		for (auto point : helper.getNeighbors(origin.x, origin.y, 1000, 1000, info.area))
		{
			area.push_back({ point.first,point.second });
		}
		return area;
	}
	return { { -1,-1 } };
}

std::vector<sf::Vector2i> Ability::getActiveTiles(sf::Vector2i pPos, sf::Vector2i mPos)
{
	sf::Vector2i target;
	sf::Vector2f dir = { (float)pPos.x - (float)mPos.x, (float)pPos.y - (float)mPos.y };
	float mag = helper.magnitude(dir);
	if (mag < info.range)
		target = mPos;
	else
	{
		dir = helper.normalized(dir, mag) * (float)info.range;
		sf::Vector2i tileDir = { (int)std::ceil(std::abs(dir.x)), (int)std::ceil(std::abs(dir.y)) };
		if (dir.x < 0)
			target.x = pPos.x + tileDir.x;
		else if (dir.x >= 0)
			target.x = pPos.x - tileDir.x;
		if (dir.y < 0)
			target.y = pPos.y + tileDir.y;
		else if (dir.y >= 0)
			target.y = pPos.y - tileDir.y;
		if (target.x < 0)
			target.x = 0;
		if (target.y < 0)
			target.y = 0;
	}
	area = getArea(target);
	drawPositions.clear();
	return area;
}
void Ability::pushPosition(sf::Vector2f pos)
{
	drawPositions.push_back(pos);
}
void Ability::update(float dt)
{
	durTimer -= dt;
	if (durTimer <= 0)
	{
		this->animHandler.reset();
		this->isAlive = false;
	}
}

void Ability::draw(float dt)
{
	this->animHandler.update(dt);
	this->sprite.setTextureRect(this->animHandler.bounds);
	for (auto pos : drawPositions)
	{
		this->sprite.setPosition(pos);
		this->game->window.draw(this->sprite);
	}
}