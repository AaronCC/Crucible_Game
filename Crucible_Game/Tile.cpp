#include <SFML/Graphics.hpp>

#include "AnimationHandler.h"
#include "Tile.h"

void Tile::draw(sf::RenderWindow& window, float dt)
{
	/* Change the sprite to reflect the tile variant */
	if (tileType == TileType::STATIC)
		this->animHandler.changeAnim(this->tileVariant);

	/* Update the animation */
	this->animHandler.update(dt);

	/* Update the sprite */
	this->sprite.setTextureRect(this->animHandler.bounds);

	/* Draw the tile */
	window.draw(this->sprite);

	return;
}

void Tile::update()
{
	return;
}

std::string tileTypeToStr(TileType type)
{
	switch (type)
	{
	default:
		break;
	}
	return "";
}