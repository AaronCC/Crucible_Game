#ifndef MAP_H
#define MAP_H
#include "Game.h"
#include "Gui.h"

class Map
{
public:
	std::vector<Tile> tiles;
	int width, height;
	sf::Vector2u tileSize;
	Tile background;

	//Tile player;
	Game* game;
	
	void draw(sf::RenderWindow & window, float dt);

	void Map::loadMap();

	Map(Game* game);
	~Map();

private:
	Gui buttons;
	sf::Font testFont;
};

#endif /* MAP_H */