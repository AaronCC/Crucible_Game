#include "Game.h"
#include "GameState.h"
#include <iostream>
#include <fstream>
#include <sstream>

Game::Game()
{
	
	this->window.create(sf::VideoMode(windowSize.x, windowSize.y), "Game");
	this->window.setFramerateLimit(60);
	tileSize = sf::Vector2u(TILE_SIZE, TILE_SIZE);
	loadTextures();
	loadFonts();
	loadTiles();
}


Game::~Game()
{
}


void Game::pushState(GameState* state)
{
	this->states.push(state);
	return;
}

void Game::popState()
{
	delete this->states.top();
	this->states.pop();

	return;
}

void Game::changeState(GameState* state)
{
	if (!this->states.empty())
		popState();
	pushState(state);

	return;
}

GameState* Game::peekState()
{
	if (this->states.empty()) return nullptr;
	return this->states.top();
}

void Game::gameLoop()
{
	sf::Clock clock;

	while (this->window.isOpen())
	{
		sf::Time elapsed = clock.restart();
		float dt = elapsed.asSeconds();

		if (peekState() == nullptr) continue;
		peekState()->handleInput();
		peekState()->update(dt);
		this->window.clear(sf::Color::Black);
		peekState()->draw(dt);
		this->window.display();
	}
}

void Game::loadTiles()
{
	Animation staticAnim(0, 0, 1.0f);
	this->tileAtlas["grass0"] =
		Tile(this->tileSize, 1, texmgr.getRef("grass"),
		{ staticAnim },
			TileType::STATIC,
			TexMap("grass0", sf::Vector2u{ 0,0 }));
	this->tileAtlas["grass1"] =
		Tile(this->tileSize, 1, texmgr.getRef("grass"),
		{ staticAnim },
			TileType::STATIC,
			TexMap("grass1", sf::Vector2u{ 1,0 }));
	this->tileAtlas["grass2"] =
		Tile(this->tileSize, 1, texmgr.getRef("grass"),
		{ staticAnim },
			TileType::STATIC,
			TexMap("grass2", sf::Vector2u{ 2,0 }));

	this->tileAtlas["dungeonWall"] =
		Tile(this->tileSize, 1, texmgr.getRef("dungeon"),
		{ staticAnim },
			TileType::STATIC,
			TexMap("dungeonWall", sf::Vector2u{ 0,0 }));
	this->tileAtlas["dungeonFloor"] =
		Tile(this->tileSize, 1, texmgr.getRef("dungeon"),
		{ staticAnim },
			TileType::STATIC,
			TexMap("dungeonFloor", sf::Vector2u{ 1,0 }));

}

void Game::loadTextures()
{
	std::string line;
	std::ifstream texFile("media/Textures.txt");
	if (texFile.is_open())
	{
		while (std::getline(texFile, line))
		{
			std::istringstream iss(line);
			std::vector<std::string> texPair((std::istream_iterator<std::string>(iss)),
				std::istream_iterator<std::string>());
			texmgr.loadTexture(texPair[0], texPair[1]);
		}
		texFile.close();
	}
}

void Game::loadFonts()
{
	sf::Font font;
	if (!font.loadFromFile("media/font.ttf"))
	{
		//Error
	}
	this->fonts["main_font"] = font;

	return;
}
