#ifndef MAP_H
#define MAP_H
#include "Game.h"

class Map
{
public:
	std::vector<Tile> tiles;
	int width, height;
	sf::Vector2u tileSize;
	Tile background;

	Game* game;

	void draw(sf::RenderWindow & window, float dt);

	void Map::loadMap();

	Map(Game* game);
	~Map();
};

#endif /* MAP_H */