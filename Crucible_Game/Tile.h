#ifndef TILE_H
#define TILE_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <time.h>
#define TILE_SIZE 32
#include "AnimationHandler.h"

enum class TileType { STATIC, ANIMATED };

struct TexMap {
	TexMap(std::string name, sf::Vector2u index) { this->name = name; this->index = index; }
	std::string name;
	sf::Vector2u index;
};

std::string tileTypeToStr(TileType type);

class Tile
{
public:

	AnimationHandler animHandler;

	sf::Vector2f position;
	sf::Sprite sprite;

	sf::Uint32 unitHeight;
	sf::Uint32 unitWidth;

	TileType tileType;

	/* Tile variant, allowing for different looking versions of the
	* same tile */
	int tileVariant;

	/* Constructor */
	Tile() { }
	Tile(sf::Vector2u tileSize, const unsigned int height, sf::Texture& texture,
		const std::vector<Animation>& animations,
		const TileType tileType,
		const TexMap texMap)
	{
		std::srand(time(NULL));
		sf::Vector2u size = texture.getSize();
		this->tileType = tileType;
		this->tileVariant = 0;
		unitHeight = tileSize.y / TILE_SIZE;
		unitWidth = tileSize.x / TILE_SIZE; 
		this->sprite.setTextureRect(sf::IntRect(tileSize.x*texMap.index.x, tileSize.y*texMap.index.y, tileSize.x, tileSize.y));
		this->sprite.setOrigin(sf::Vector2f(0, 0));
		this->sprite.setTexture(texture);
		this->animHandler.frameSize = sf::IntRect(0, 0, tileSize.x, tileSize.y*height);
		for (auto animation : animations)
		{
			this->animHandler.addAnim(animation);
		}
		this->animHandler.update(0.0f);
	}

	void setPosition(sf::Vector2f position) { this->position = position; }

	void draw(sf::RenderWindow& window, float dt);

	void update();
};

#endif /* TILE_H */