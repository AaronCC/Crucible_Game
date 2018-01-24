#include "Ability.h"

Ability::~Ability()
{
}

void Ability::activate(sf::Vector2f pPos, sf::Vector2i mPos, std::string id)
{
 	this->isAlive = true;
	this->durTimer = this->duration;
	this->animHandler.changeAnim(0);
	this->animHandler.reset();
	this->sprite.setPosition(pPos);
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
