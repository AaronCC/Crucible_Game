#include "TestState.h"

TestState::TestState(Game* game)
{
	this->game = game;
	testFont.loadFromFile("C:/Windows/Fonts/Arial.ttf");
	testText.setFont(testFont);
	testText.setPosition(200, 200);
	testText.setString("Crucible Game");
	initView();
	map = new Map(game);
	map->loadMap();
}

TestState::~TestState()
{
}

void TestState::draw(const float dt)
{
	this->game->window.setView(this->view);
	this->game->window.draw(testText);
	this->map->draw(this->game->window, dt);
}

void TestState::update(const float dt)
{
}

void TestState::handleInput()
{
	sf::Event event;

	sf::Vector2f mousePos = this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->view);

	while (this->game->window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Resized:
		{
			resizeView(event.size.width, event.size.height);
			break;
		}
		case sf::Event::KeyPressed:
		{
			
		}
		default: break;
		}
	}
	sf::Vector2f center = view.getCenter();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		updateView({ center.x - 5, center.y });
		this->map->player.animHandler.changeAnim(1);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		updateView({ center.x, center.y - 5 });
		this->map->player.animHandler.changeAnim(0);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		updateView({ center.x, center.y + 5});
		this->map->player.animHandler.changeAnim(2);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		updateView({ center.x + 5, center.y });
		this->map->player.animHandler.changeAnim(3);
	}
}
