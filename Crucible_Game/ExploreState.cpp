#include "ExploreState.h"
#include <queue>
#define PLAYER this->map->player

ExploreState::ExploreState(Game* game)
{
	this->game = game;
	testFont.loadFromFile("C:/Windows/Fonts/Arial.ttf");
	testText.setFont(testFont);
	testText.setPosition(0, 0);
	testText.setString("");
	initView();
	Animation walkAnim(0, 0, 0.1);
	camera = Camera(game, &player);
	map = new Map(game, &camera);
	map->loadMap();
	player = Player(game,
		sf::Vector2u(32, 32),
		this->game->texmgr.getRef("player"),
		{ walkAnim,walkAnim,walkAnim,walkAnim },
		map->spawnPos, &this->camera.view, &this->map->mouseIndex);
	pf = PathFinder(this->map, this->map->width, this->map->height);
	this->old_mLeftState = true;
	this->player.updateTilePos();
	resolveFoW();
	rTime = 0.01f;
}

ExploreState::~ExploreState()
{
}

void ExploreState::draw(const float dt)
{
	this->camera.setView();
	//this->game->window.
	this->map->draw(this->game->window, dt);
	if (fTime >= 1)
	{
		fTime = 0;
		fTotal = fps;
		fps = 0;
	}
	fTime += dt;
	fps++;
	testText.setString("FPS: " + std::to_string(fTotal));

	this->player.draw(dt);
	//this->game->window.draw(testText);
}

void ExploreState::update(const float dt)
{
	sf::Vector2f oldPos = player.position;
	this->player.update(dt);
	this->camera.update(dt);
	this->map->update(dt);
	this->player.updateAnim(this->camera.view);
	if (player.resolveActions)
		resolveGameState(player.tickCount);
	if (resolving.first)
	{
		rTimeTotal += dt;
		if (rTimeTotal >= rTime)
		{
			std::queue<std::string> msgs;
			if (player.queuedAction == Player::Action::ABILITY)
			{
				std::vector<AbEffect::Effect> abEffs = player.queuedAbility->getEffects();
				for (auto point : player.getQueuedPoints())
				{
					std::vector<Enemy*> enemies = map->getEnemiesAtPoint(point);
					for (auto enemy : enemies)
					{
						for (auto eff : abEffs)
						{
							enemy->applyEff(eff);
						}
					}
					if (map->getTile(point.x, point.y)->passable)
					{
						sf::Vector2i los = map->hasLineOfSight(player.tilePos, point);
						player.resolveAbilityOnTile(los);
					}
				}
				resolving = { false,0 };
				player.clearQueuedPoints();
				msgs.push(player.activateQueuedAbility());
				player.resolveAbilityCDs(resolving.second);
			}
			else if (player.queuedAction == Player::Action::MOVE)
			{
				if (player.moveNext())
				{
					resolveFoW();
					msgs.push("Moved");
				}
				else
				{
					this->player.clearWayPoints();
					player.resolveAbilityCDs(resolving.second);
					resolving = { false,0 };
				}
			}
			rTimeTotal = 0;
			player.queueHudMsg(msgs);
		}
	}
}

void ExploreState::handleInput()
{
	sf::Event event;
	std::queue<std::string> msgs;
	while (this->game->window.pollEvent(event))
	{
		player.handleEvent(event);
		switch (event.type)
		{
		case sf::Event::Resized:
		{
			this->camera.resizeView(event.size.width, event.size.height);
			break;
		}
		case sf::Event::KeyReleased:
		{

		}
		default: break;
		}
	}
	sf::Vector2f mousePos = this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->view);

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && old_mLeftState == false)
	{
		if (player.hud.showState == Hud::ShowState::SHOW_INV && (player.inventory.showInfo || player.inventory.delHover))
		{
				player.inventory.select();
		}
		if (!player.engaged)
		{
			if (player.hud.showState == Hud::ShowState::SHOW_NONE)
			{
				std::vector<std::pair<int, int>> path = pf.findPath(this->player.tilePos, this->map->mouseIndex);
				this->player.clearWayPoints();
				for (auto point : path)
				{
					this->player.addWayPoint(point);
				}

				this->player.queuedAction = Player::Action::MOVE;
			}
		}
		else
		{
			msgs.push("Cannot quick-move during combat. Use WSAD.");
		}
		old_mLeftState = true;
	}
	else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
		old_mLeftState = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && old_wState == false)
	{
		std::pair<int, int> point = { player.tilePos.x,player.tilePos.y - 1 };
		if (map->getTile(point.first, point.second)->passable)
		{
			this->player.clearWayPoints();
			this->player.addWayPoint(point);
			this->player.queuedAction = Player::Action::MOVE;
		}
		old_wState = true;
	}
	else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		old_wState = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && old_sState == false)
	{
		std::pair<int, int> point = { player.tilePos.x,player.tilePos.y + 1 };
		if (map->getTile(point.first, point.second)->passable)
		{
			this->player.clearWayPoints();
			this->player.addWayPoint(point);
			this->player.queuedAction = Player::Action::MOVE;
		}
		old_sState = true;
	}
	else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		old_sState = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && old_aState == false)
	{
		std::pair<int, int> point = { player.tilePos.x - 1,player.tilePos.y };
		if (map->getTile(point.first, point.second)->passable)
		{
			this->player.clearWayPoints();
			this->player.addWayPoint(point);
			this->player.queuedAction = Player::Action::MOVE;
		}
		old_aState = true;
	}
	else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		old_aState = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && old_dState == false)
	{
		std::pair<int, int> point = { player.tilePos.x + 1,player.tilePos.y };
		if (map->getTile(point.first, point.second)->passable)
		{
			this->player.clearWayPoints();
			this->player.addWayPoint(point);
			this->player.queuedAction = Player::Action::MOVE;
		}
		old_dState = true;
	}
	else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		old_dState = false;
	this->player.handleInput();
	if (player.checkLineOfSight)
	{
		player.resolveLineOfSight(map->hasLineOfSight(player.tilePos, map->mouseIndex));
		player.checkLineOfSight = false;
	}
	sf::Vector2f center = view.getCenter();

	player.queueHudMsg(msgs);
}

void ExploreState::resolveFoW()
{
	std::vector<sf::Vector2i> points;
	for (auto tile : fowCache)
	{
		tile->fow = true;
	}
	fowCache.clear();
	for (int y = -player.lightRadius; y <= player.lightRadius; y++)
	{
		for (int x = -player.lightRadius; x <= player.lightRadius; x++)
		{
			sf::Vector2i pos = { x + player.tilePos.x, y + player.tilePos.y };
			if (pos.x < 0 || pos.y < 0 || pos.x > map->width - 1 || pos.y > map->height - 1)
				continue;
			Tile* tile = map->getTile(pos.x, pos.y);
			tile->reveal();
			points.push_back({ pos.x,pos.y });
			fowCache.push_back(tile);
		}
	}
	if (map->activateObjsAtTile(points) && !player.engaged)
	{
		player.engaged = true;
		player.clearWayPoints();
	}
	else
		player.engaged = false;
}

void ExploreState::resolveGameState(unsigned int ticks)
{
	resolving = { true,ticks };
	/*
	std::queue<std::string> msgs;
	if (player.queuedAction == Player::Action::ABILITY)
		msgs.push(player.activateQueuedAbility());
	else if (player.queuedAction == Player::Action::MOVE)
		while (player.moveNext())
		{
			resolveFoW();
			msgs.push("Moved");
		}
	player.resolveAbilityCDs(ticks);
	player.queueHudMsg(msgs);
	this->player.clearWayPoints();
	*/
}

