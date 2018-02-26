#include "Map.h"
#include <time.h>
#include "Dungeon.h"


void Map::draw(sf::RenderWindow & window, float dt)
{

	// Get the camera's position
	sf::Vector2f camPos = this->camera->view.getCenter();
	sf::Vector2f center = camPos / 32.f;
	// Calculate the start tile
	sf::Vector2f drawStart = { (int)center.x - (drawSize.x / 2), (int)center.y - (drawSize.y / 2) };
	// Clamp
	drawStart.x = drawStart.x < 0 ? 0 : drawStart.x;
	drawStart.y = drawStart.y < 0 ? 0 : drawStart.y;
	// Calc end tile
	sf::Vector2f drawEnd = { drawStart.x + drawSize.x, drawStart.y + drawSize.y };
	// Clamp
	drawEnd.x = drawEnd.x > width ? width : drawEnd.x;
	drawEnd.y = drawEnd.y > height ? height : drawEnd.y;

	for (int y = drawStart.y; y < drawEnd.y; ++y)
	{
		for (int x = drawStart.x; x < drawEnd.x; ++x)
		{
			this->tiles[y*this->width + x].draw(window, dt);
		}
	}
	if (this->tiles[mouseIndex.y*this->width + mouseIndex.x].passable)
		canSelect.draw(window, dt);
	else
		cantSelect.draw(window, dt);
	return;

}

void Map::update(float dt)
{
	sf::Vector2i selectPos = getSelectPos();
	canSelect.setPosition(selectPos);
	cantSelect.setPosition(selectPos);
}

sf::Vector2i Map::getSelectPos()
{
	sf::Vector2f camPos = this->camera->view.getCenter();
	sf::Vector2f camSize = this->camera->view.getSize();
	sf::Vector2f mousePos = 
		this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->camera->view);
	mousePos -= {16, 16};
	this->mouseIndex.x = std::ceil(mousePos.x / 32);
	this->mouseIndex.y = std::ceil(mousePos.y / 32);
	if (mouseIndex.x < 0)
		mouseIndex.x = 0;
	else if (mouseIndex.x > width)
		mouseIndex.x = width - 1;
	if (mouseIndex.y < 0)
		mouseIndex.y = 0;
	else if (mouseIndex.y >= height)
		mouseIndex.y = height - 1;
	return { mouseIndex.x * 32, mouseIndex.y * 32 };
}

void Map::handleInput(sf::Event event)
{
	switch (event.type)
	{
	case sf::Event::MouseMoved:

		break;
	default:
		break;
	}
}

void Map::loadMap()
{
	width = 79;
	height = 24;
	Dungeon dungeon(width, height);
	dungeon.generate(70);
	dungeon.print();
	tiles.reserve(width * height);
	sf::Vector2i pos = { 0,0 };
	this->spawnPos = { width / 2, (height / 2) + 1 };
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; x++)
		{
			char t = dungeon.getTile(x, y);
			this->tiles.push_back(game->tileAtlas.at("dungeon"));
			switch (t)
			{
			case Dungeon::Tile::Unused:
			{
				this->tiles[y*this->width + x].tileVariant = 0;
				break;
			}
			case Dungeon::Tile::Floor:
			{
				this->tiles[y*this->width + x].tileVariant = 1;
				this->tiles[y*this->width + x].passable = false;
				break;
			}
			case Dungeon::Tile::Wall:
			{
				this->tiles[y*this->width + x].tileVariant = 2;
				this->tiles[y*this->width + x].passable = false;
				break;
			}
			case Dungeon::Tile::ClosedDoor:
			{
				this->tiles[y*this->width + x].tileVariant = 3;
				break;
			}
			case Dungeon::Tile::OpenDoor:
			{
				this->tiles[y*this->width + x].tileVariant = 4;
				break;
			}
			case Dungeon::Tile::UpStairs:
			{
				this->tiles[y*this->width + x].tileVariant = 5;
				break;
			}
			case Dungeon::Tile::DownStairs:
			{
				this->tiles[y*this->width + x].tileVariant = 6;
				break;
			}
			default:
				break;
			}

			this->tiles[y*this->width + x].setPosition(pos, x, y);
			this->tiles[y*this->width + x].sprite.setPosition((sf::Vector2f)pos);
			pos.x += tileSize.x;
		}
		pos.x -= tileSize.x * width;
		pos.y += tileSize.y;
	}
}

Tile* Map::getTile(int x, int y)
{
	return &this->tiles[y*this->width + x];
}

Map::Map(Game* game, Camera* camera)
{
	this->game = game;
	this->camera = camera;
	this->tiles = std::vector<Tile>();
	this->tileSize = this->game->tileSize;
	this->canSelect = this->game->tileAtlas.at("can_select");
	this->cantSelect = this->game->tileAtlas.at("cant_select");
	this->canSelect.reveal();
	this->cantSelect.reveal();
	this->drawSize = { (float)this->game->windowSize.x / tileSize.x, (float)this->game->windowSize.y / tileSize.y };
}
Map::~Map()
{
}
