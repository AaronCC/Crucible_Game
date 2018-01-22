#pragma once
#include "AnimationHandler.h"
#include <SFML/Graphics.hpp>
#include "Game.h"

class Player
{
public:
	
	AnimationHandler animHandler;

	Game* game;

	sf::Sprite sprite;

	std::map<sf::Keyboard::Key, bool> keys;
	std::map<std::string, unsigned int> anims;

	void handleInput();
	void draw(float dt);
	void update(float dt);
	void move(sf::Vector2f offset) { position += offset; }
	void setPos(sf::Vector2f pos) { position = pos; }
	sf::Vector2f getForce() { return totalForce; }
	Player() {};
	Player(Game* game, sf::Vector2u size, sf::Texture& texture, const std::vector<Animation>& animations)
	{
		this->game = game;
		this->friction = 0.5f;
		this->mass = 1.f;
		this->speed = 1.f;
		this->maxSpeed = 2.f;
		this->position = { (float)game->windowSize.x / 2, (float)game->windowSize.y / 2 };
		this->animHandler.changeAnim(0);
		this->sprite.setOrigin(sf::Vector2f(size.x / 2, size.y));
		this->sprite.setTexture(texture);
		this->animHandler.frameSize = sf::IntRect(0, 0, size.x, size.y);
		for (auto animation : animations)
		{
			this->animHandler.addAnim(animation);
		}
		this->animHandler.update(0.0f);
		anims["N"] = 0;
		anims["W"] = 1;
		anims["S"] = 2;
		anims["E"] = 3;
		keys[sf::Keyboard::Left] = false;
		keys[sf::Keyboard::Right] = false;
		keys[sf::Keyboard::Up] = false;
		keys[sf::Keyboard::Down] = false;
	}

	~Player();

private:
	sf::Vector2f position;
	sf::Vector2f moveForce;
	sf::Vector2f totalForce;
	sf::Vector2f acceleration;
	sf::Vector2f velocity;

	float maxSpeed;
	float friction;
	float speed;
	float mass;

};

