#include "Player.h"

void Player::handleInput()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		moveForce.x -= speed;
		walkState = WalkState::LEFT;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		moveForce.y -= speed;
		walkState = WalkState::UP;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		moveForce.y += speed;
		walkState = WalkState::DOWN;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		moveForce.x += speed;
		walkState = WalkState::RIGHT;

	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		if (rmbCooldown <= 0)
		{
			Ability* ability = new Ability(rmbAbility);
			ability->activate(this->position, sf::Mouse::getPosition(), ability->id);
			abilities.push_back(ability);
			rmbCooldown = ability->cooldown;
		}
	}
	/* temp */
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))
	{
		health -= 1;
		hud.updateHealth(health / (float)maxHealth);
		if (health <= 0)
		{
			health = maxHealth;
		}
	}

}

void Player::handleEvent(sf::Event event)
{
	switch (event.type)
	{
	case sf::Event::Resized:
		hudView.setViewport(helper.resizeView(event.size.width, event.size.height, game->aspectRatio));

		break;
	default:
		break;
	}
}

void Player::draw(float dt)
{
	/* Update the animation */
	this->animHandler.update(dt);

	/* Update the sprite */
	this->sprite.setTextureRect(this->animHandler.bounds);

	/* Draw the tile */
	this->game->window.draw(this->sprite);

	for (auto ability : abilities)
	{
		ability->draw(dt);
	}

	this->game->window.setView(hudView);
	this->hud.draw(dt);

	return;
}

void Player::updateAnim(sf::View view)
{
	this->animHandler.unPause();

	if (walkState == WalkState::NONE && velocity.x == 0 && velocity.y == 0 && !this->animHandler.isPaused())
	{
		this->animHandler.reset();
		this->animHandler.pause();
	}

	sf::Vector2f mousePos = this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), view);
	std::string oldAnim = currentAnim;

	/*sf::Vector2f camPos = view.getCenter();
	sf::Vector2f camW = view.getSize() / 2.f;
	mousePos = { mousePos.x + camPos.x - camW.x, mousePos.y + camPos.y - camW.y};*/

	float xdiff = mousePos.x - position.x;
	float ydiff = mousePos.y - position.y;

	if (std::abs(ydiff) >= std::abs(xdiff))
	{
		if (ydiff < 0)
			currentAnim = "N";
		else
			currentAnim = "S";
	}
	else if (xdiff > 0)
	{
		if (std::abs(ydiff) < std::abs(xdiff))
			currentAnim = "E";
	}
	else if (xdiff < 0)
	{
		if (std::abs(ydiff) < std::abs(xdiff))
			currentAnim = "W";
	}

	this->animHandler.changeAnim(anims[currentAnim]);
	oldWalkState = walkState;
	walkState = WalkState::NONE;
}

void Player::update(float dt)
{
	float fX, fY;

	totalForce += moveForce;

	fX = velocity.x == 0 ? 0 : friction;
	fY = velocity.y == 0 ? 0 : friction;

	acceleration.x = velocity.x > 0 ? (totalForce.x / mass) - fX
		: (totalForce.x / mass) + fX;
	acceleration.y = velocity.y > 0 ? (totalForce.y / mass) - fY
		: (totalForce.y / mass) + fY;

	velocity += acceleration;

	velocity.x = std::abs(velocity.x) <= std::abs(fX) ? 0 : velocity.x;
	velocity.y = std::abs(velocity.y) <= std::abs(fY) ? 0 : velocity.y;

	velocity = helper.clamp(velocity, maxSpeed);

	move(velocity * dt);
	this->sprite.setPosition(position);

	totalForce -= moveForce;
	moveForce = { 0,0 };

	updateAbilities(dt);
}

void Player::updateAbilities(float dt)
{
	int i = 0;
	std::vector<int> rmCache;
	if (rmbCooldown > 0)
		rmbCooldown -= dt;
	if (lmbCooldown > 0)
		lmbCooldown -= dt;
	for (auto cd : keyCooldowns)
	{
		if (cd.second > 0)
			cd.second -= dt;
	}
	for (auto ability : abilities)
	{
		ability->update(dt);
		if (!ability->isAlive)
			rmCache.push_back(i);
		i++;
	}
	for (int rm = 0; rm < rmCache.size(); rm++)
	{
		abilities.erase(abilities.begin() + rmCache[rm]);
		if (rm < rmCache.size() - 1)
			rmCache[rm + 1]--;
	}
}

Player::~Player()
{
}
