#ifndef PLAYER_H
#define PLAYER_H

#include "AnimationHandler.h"
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Hud.h"
#include "Helper.h"
#include "Ability.h"

class Player
{
public:
	
	enum WalkState {
		LEFT,
		UP,
		DOWN,
		RIGHT,
		NONE
	};
	
	int health;
	int maxHealth;

	WalkState walkState;
	WalkState oldWalkState;
		
	AnimationHandler animHandler;

	Game* game;
	Hud hud;
	sf::View hudView;

	sf::Sprite sprite;

	std::vector<Ability*> abilities;
	std::map<sf::Keyboard::Key, Ability> keyAbilities;
	std::map<sf::Keyboard::Key, float> keyCooldowns;
	Ability rmbAbility;
	float rmbCooldown;
	Ability lmbAbility;
	float lmbCooldown;

	std::map<sf::Keyboard::Key, bool> keys;
	std::map<std::string, unsigned int> anims;

	sf::Vector2f position;

	Helper helper;

	void handleInput();
	void handleEvent(sf::Event event);
	void draw(float dt);
	void update(float dt);
	void updateAbilities(float dt);
	void move(sf::Vector2f offset) { position += offset; }
	void setPos(sf::Vector2f pos) { position = pos; }
	void updateAnim(sf::View view);

	sf::Vector2f getForce() { return totalForce; }

	Player() {};
	Player(Game* game, sf::Vector2u size, sf::Texture& texture, const std::vector<Animation>& animations)
	{
		walkState = WalkState::NONE;
		this->game = game;

		this->friction = 15.f;
		this->mass = 1.f;
		this->speed = 50.f;
		this->maxSpeed = 100.f;

		this->health = 100;
		this->maxHealth = this->health;

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

		hudView = { sf::Vector2f(this->game->windowSize.x / 2, this->game->windowSize.y / 2),
			sf::Vector2f(this->game->windowSize.x, this->game->windowSize.y) };

		hud = Hud(game, { "globe","life_globe", "ability_slot" });

		/* TEMP */
		rmbAbility = Ability(this->game, game->texmgr.getRef("slash"), { 0,3,0.1f }, { 32,32 }, "slash", 0.5f);
		hud.setSlotSprites({}, "", "slash_icon");
	}

	~Player();

private:
	sf::Vector2f moveForce;
	sf::Vector2f totalForce;
	sf::Vector2f acceleration;
	sf::Vector2f velocity;
	std::string currentAnim;

	float maxSpeed;
	float friction;
	float speed;
	float mass;

};

#endif /* PLAYER_H */