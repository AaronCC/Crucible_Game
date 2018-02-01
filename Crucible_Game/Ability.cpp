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

std::vector<sf::Vector2i> Ability::getActiveTiles(sf::Vector2i pPos, sf::Vector2i mPos)
{
	switch (id)
	{
	case ID::SLASH:
		this->sprite.setPosition(mPos.x * 32, mPos.y * 32);
		return { mPos };
	default:
		break;
	}
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
	this->game->window.draw(this->sprite);
}
