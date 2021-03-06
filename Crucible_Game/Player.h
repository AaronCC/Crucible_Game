#ifndef PLAYER_H
#define PLAYER_H

#include "AnimationHandler.h"
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Hud.h"
#include "Helper.h"
#include "Ability.h"
#include <stack>

class Player
{
public:
	
	enum WalkState {
		LEFT,
		UP,
		DOWN,
		RIGHT
		//NONE
	};
	
	enum Action {
		MOVE,
		ABILITY,
		NONE
	};

	Action queuedAction;

	int health;
	int maxHealth;

	unsigned int tickCount;
	bool resolveActions;
	bool engaged;

	WalkState walkState;
	WalkState oldWalkState;
		
	AnimationHandler animHandler;

	Game* game;
	Hud hud;
	sf::View hudView;

	Helper::Stats stats;

	float testTimer = 0;

	sf::View* view;
	sf::Vector2i* mIndex;

	int lightRadius;

	sf::Sprite sprite;
	sf::Sprite queueSprite;

	Ability* queuedAbility;
	std::vector<Ability*> abilities;

	std::map<sf::Keyboard::Key, Ability> keyAbilities;
	std::map<sf::Keyboard::Key, float> keyCooldowns;

	Ability rmbAbility;
	float rmbCooldown;
	Ability lmbAbility;
	float lmbCooldown;
	float queuedCooldown;
	unsigned int queuedAbilitySlotNum;

	std::map<sf::Keyboard::Key, bool> keys;
	std::map<std::string, unsigned int> anims;

	sf::Vector2f position;
	sf::Vector2i tilePos;

	Helper helper;
	void resolveLineOfSight(sf::Vector2i los);
	void resolveAbilityOnTile(sf::Vector2i pos);
	void handleInput();
	void queueAbility(int slotIndex);
	void handleEvent(sf::Event event);
	void draw(float dt);
	void update(float dt);
	void updateTilePos();
	void updateAbilities(float dt);
	void move(sf::Vector2f offset) { 
		position += offset;
		position.x = std::round(position.x);
		position.y = std::round(position.y);
	}
	void setPos(sf::Vector2f pos) { 
		position = pos; 
		position.x = std::round(position.x);
		position.y = std::round(position.y);
		this->sprite.setPosition(position);
	}
	void updateAnim(sf::View view);

	bool checkLineOfSight = false;

	sf::Vector2f getForce() { return totalForce; }
	void addQueuedPoint(sf::Vector2i point)
	{
		queuedPoints.push_back({ point.x, point.y });
	}
	void clearQueuedPoints()
	{
		queuedPoints.clear();
	}
	std::vector<sf::Vector2i> getQueuedPoints()
	{
		return this->queuedPoints;
	}
	void addWayPoint(std::pair<int, int> point) { 
		queuedPoints.push_back({ point.first, point.second });
		wayPoints.push({ point.first,point.second }); 
		this->tickCount += speed * 1;
	}
	void clearWayPoints() {
		this->tickCount = 0;
		this->wayPoints = std::stack<sf::Vector2i>();
		queuedPoints.clear();
	}

	std::string activateQueuedAbility()
	{
		std::string msg = queuedAbility->name;
		queuedAbility->activate(tilePos, *mIndex);
		abilities.push_back(queuedAbility);
		hud.setCooldown(queuedAbilitySlotNum, queuedCooldown);
		queuedAbility = nullptr;
		queuedAction = Action::NONE;
		return msg;
	}
	void resolveAbilityCDs(unsigned int ticks)
	{
		for (int cd = 0; cd < hud.cooldowns.size(); cd++)
		{
			if(hud.cooldowns[cd].active)
				hud.updateCD(cd, ticks);
		}
	}

	bool moveNext();

	Inventory inventory;

	void queueHudMsg(std::queue<std::string> msgs);

	Player() {};
	Player(Game* game, sf::Vector2u size,
		sf::Texture& texture, const std::vector<Animation>& animations,
		sf::Vector2i spawnPos, sf::View* view, sf::Vector2i* mIndex)
	{
		//walkState = WalkState::NONE;
		this->mIndex = mIndex;
		this->view = view;
		this->game = game;
		this->lightRadius = 4;
		this->resolveActions = false;
		this->tickCount = 0;
		this->friction = 15.f;
		//this->mass = 1.f;
		this->speed = 1.f;
		//this->maxSpeed = 100.f;

		this->health = 100;
		this->maxHealth = this->health;

		this->position = { spawnPos.x * (float)game->tileSize.x, spawnPos.y * (float)game->tileSize.y};
		this->animHandler.changeAnim(0);
		this->sprite.setOrigin(sf::Vector2f(size.x / 2, size.y/2));
		this->sprite.setTexture(texture);
		this->animHandler.frameSize = sf::IntRect(0, 0, size.x, size.y);
		for (auto animation : animations)
		{
			this->animHandler.addAnim(animation);
		}
		this->animHandler.update(0.0f);
		anims["N"] = 2;
		anims["W"] = 3;
		anims["S"] = 0;
		anims["E"] = 1;
		keys[sf::Keyboard::Left] = false;
		keys[sf::Keyboard::Right] = false;
		keys[sf::Keyboard::Up] = false;
		keys[sf::Keyboard::Down] = false;
		keys[sf::Keyboard::Space] = false;

		hudView = { sf::Vector2f(this->game->windowSize.x / 2, this->game->windowSize.y / 2),
			sf::Vector2f(this->game->windowSize.x, this->game->windowSize.y) };

		hud = Hud(game, { "globe","life_globe", "ability_slot", "can_select", "cant_select" });

		/* TEMP */
		rmbAbility = Ability(this->game, game->texmgr.getRef("slash"),"slash",
			{ 0,3,0.1f }, { 32,32 }, Ability::ID::SLASH, 10, 2, "slash", "Slash");
		hud.setSlotSprites({}, "move_icon", "slash_icon");

		queueSprite.setTexture(this->game->texmgr.getRef("queue_select"));
		queueSprite.setOrigin(this->game->tileSize.x/2,this->game->tileSize.y/2);
		queueSprite.setTextureRect(sf::IntRect( 0,0,this->game->tileSize.x,this->game->tileSize.y ));

		setPos(position);

		this->inventory = Inventory(this->game);

		this->engaged = false;

		updateAbilityBar();
	}

	void updateAbilityBar();

	~Player();

private:
	sf::Vector2f moveForce;
	sf::Vector2f totalForce;
	sf::Vector2f acceleration;
	sf::Vector2f velocity;
	std::string currentAnim;
	std::stack<sf::Vector2i> wayPoints;
	std::vector<sf::Vector2i> queuedPoints;

	float maxSpeed;
	float friction;
	float speed;
	float mass;

};

#endif /* PLAYER_H */