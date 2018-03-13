#ifndef MAP_H
#define MAP_H
#include "Game.h"
#include "Gui.h"
#include "Camera.h"
#include "Helper.h"
#include "Enemy.h"
class Map
{
public:
	std::vector<Enemy> enemies;
	std::vector<Tile> tiles;
	std::vector<Tile>* getTiles() { return &tiles; }
	int width, height;
	sf::Vector2u tileSize;
	sf::Vector2i spawnPos;
	sf::Vector2f drawSize;
	Tile background;
	Tile canSelect;
	Tile cantSelect;

	//Tile player;
	Game* game;

	Camera* camera;

	sf::Vector2i getSelectPos();
	sf::Vector2i mouseIndex;
	Helper helper;
	sf::Vector2i hasLineOfSight(sf::Vector2i from, sf::Vector2i to);

	void draw(sf::RenderWindow & window, float dt);
	sf::Vector2i globalToTilePos(sf::Vector2f global);
	void update(float dt);
	bool activateObjsAtTile(std::vector<sf::Vector2i> pos);
	void handleInput(sf::Event event);

	std::vector<Enemy*> getEnemiesAtPoint(sf::Vector2i point);


	void Map::loadMap();

	Tile * getTile(int x, int y);

	Map(Game* game, Camera* camera);
	~Map();

private:
	Gui buttons;
	sf::Font testFont;
};

#endif /* MAP_H */