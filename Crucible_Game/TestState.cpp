#include "TestState.h"

#define PLAYER this->map->player

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
		case sf::Event::KeyReleased:
		{
			
		}
		default: break;
		}
	}
	sf::Vector2f center = view.getCenter();
	PLAYER.animHandler.unPause();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		updateView({ center.x - 2, center.y });
		PLAYER.animHandler.changeAnim(1);
		PLAYER.sprite.setPosition({ PLAYER.sprite.getPosition().x - 2, PLAYER.sprite.getPosition().y });
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		updateView({ center.x, center.y - 2 });
		PLAYER.animHandler.changeAnim(0);
		PLAYER.sprite.setPosition({ PLAYER.sprite.getPosition().x, PLAYER.sprite.getPosition().y - 2 });
		
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		updateView({ center.x, center.y + 2});
		PLAYER.animHandler.changeAnim(2);
		PLAYER.sprite.setPosition({ PLAYER.sprite.getPosition().x, PLAYER.sprite.getPosition().y + 2 });
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		updateView({ center.x + 2, center.y });
		PLAYER.animHandler.changeAnim(3);
		PLAYER.sprite.setPosition({ PLAYER.sprite.getPosition().x + 2, PLAYER.sprite.getPosition().y });
	}
	else
	{
		PLAYER.animHandler.pause();
	}
}
