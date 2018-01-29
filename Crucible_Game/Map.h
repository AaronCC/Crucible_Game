#ifndef MAP_H
#define MAP_H
#include "Game.h"
#include "Gui.h"
#include "Camera.h"

class Map
{
public:
	std::vector<Tile> tiles;
	std::vector<Tile>* getTiles() { return &tiles; }
	int width, height;
	sf::Vector2u tileSize;
	sf::Vector2i spawnPos;
	Tile background;
	Tile canSelect;
	Tile cantSelect;

	//Tile player;
	Game* game;

	Camera* camera;
	
	sf::Vector2i getSelectPos();
	sf::Vector2i mouseIndex;

	void draw(sf::RenderWindow & window, float dt);
	void update(float dt);
	void handleInput(sf::Event event);

	void Map::loadMap();

	Tile * getTile(int x, int y);

	Map(Game* game, Camera* camera);
	~Map();

private:
	Gui buttons;
	sf::Font testFont;
};

#endif /* MAP_H */