#include "ExploreState.h"

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
	this->game->window.draw(testText);
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
}

void ExploreState::handleInput()
{
	sf::Event event;

	sf::Vector2f mousePos = this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->view);

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && old_mLeftState == false)
	{
		std::vector<std::pair<int, int>> path = pf.findPath(this->player.tilePos, this->map->mouseIndex);
		this->player.clearWayPoints();
		for (auto point : path)
		{
			this->player.addWayPoint(point);
		}

		this->player.queuedAction = Player::Action::MOVE;
		//this->player.setPos((sf::Vector2f)this->map->mouseIndex * 32.f);
		old_mLeftState = true;
	}
	else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
		old_mLeftState = false;

	this->player.handleInput();
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
	sf::Vector2f center = view.getCenter();
}

void ExploreState::resolveFoW()
{
	for (int y = -player.lightRadius; y <= player.lightRadius; y++)
	{
		for (int x = -player.lightRadius; x <= player.lightRadius; x++)
		{
			sf::Vector2i pos = { x + player.tilePos.x, y + player.tilePos.y };
			if (pos.x < 0 || pos.y < 0 || pos.x > map->width - 1 || pos.y > map->height - 1)
				continue;
			map->getTile(pos.x, pos.y)->reveal();
		}
	}
}

void ExploreState::resolveGameState(unsigned int ticks)
{
	if (player.queuedAction == Player::Action::ABILITY)
		player.activateQueuedAbility();
	else if (player.queuedAction == Player::Action::MOVE)
		while (player.moveNext())
		{
			resolveFoW();
		}
	player.resolveAbilityCDs(ticks);
	this->player.clearWayPoints();
}

