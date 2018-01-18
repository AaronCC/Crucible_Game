#include "Map.h"
#include <time.h>
void Map::draw(sf::RenderWindow & window, float dt)
{
	for (int y = 0; y < this->height; ++y)
	{
		for (int x = 0; x < this->width; ++x)
		{
			this->tiles[y*this->width + x].draw(window, dt);
		}
	}
	player.draw(window, dt);
	return;
}

void Map::loadMap()
{
	std::srand(time(NULL));
	/*
	----------------
	TEST MAP
	5x5 GRASS @ 0,0

	width = 26;
	height = 26;
	sf::Vector2f origin{ 0,0 };
	sf::Vector2f pos{ origin.x,origin.y };
	for (size_t y = 0; y < this->height; ++y)
	{
		for (size_t x = 0; x < this->width; ++x)
		{
			int ind = std::rand() % 3;
			tiles.push_back(game->tileAtlas.at("grass"+std::to_string(ind)));

			this->tiles[y*this->width + x].setPosition(pos);
			this->tiles[y*this->width + x].sprite.setPosition(pos);

			pos.x += tileSize.x;
		}
		pos.y += tileSize.y;
		pos.x = origin.x;
	}

	END
	----------------
	DUNGEON LEVEL

	*/
	width = 25;
	height = 25;
	sf::Vector2f origin{ 0,0 };
	sf::Vector2f pos{ origin.x,origin.y };

	for (size_t y = 0; y < height; y++)
	{
		for (size_t x = 0; x < width; x++)
		{
			tiles.push_back(game->tileAtlas.at("dungeon"));
			if (x == 0 || y == 0 || x == width - 1 || y == height - 1)
				this->tiles[y*this->width + x].tileVariant = 0;
			else
				this->tiles[y*this->width + x].tileVariant = 1;
			this->tiles[y*this->width + x].setPosition(pos);
			this->tiles[y*this->width + x].sprite.setPosition(pos);
			pos.x += tileSize.x;
		}
		pos.y += tileSize.y;
		pos.x = origin.x;
	}
	this->player = game->tileAtlas.at("walktest");
	player.setPosition(sf::Vector2f(0, 0));
	player.sprite.setPosition(sf::Vector2f(0, 0));
	player.animHandler.changeAnim(0);
}

Map::Map(Game* game)
{
	this->game = game;
	this->tiles = std::vector<Tile>();
	loadMap();
	tileSize = this->game->tileSize;
}
Map::~Map()
{
}
