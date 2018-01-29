#include "Player.h"
//#include "PathFinder.h"

#include <cmath>
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
			hud.setCooldown(7, rmbCooldown);
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
	case sf::Event::MouseLeft:

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
	//this->animHandler.unPause();

	//if (walkState == WalkState::NONE && velocity.x == 0 && velocity.y == 0 && !this->animHandler.isPaused())
	//{
	//	this->animHandler.reset();
	//	this->animHandler.pause();
	//}

	sf::Vector2f mousePos = this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), view);
	std::string oldAnim = currentAnim;

	float xdiff = mousePos.x - position.x;
	float ydiff = mousePos.y - position.y;
	sf::Vector2f normalDir = helper.normalized({ xdiff,ydiff },
		helper.magnitude({ xdiff,ydiff }));
	double angle = std::acos(helper.dotProduct({ 0,1 }, normalDir))*(180.0 / 3.141592653589793238463);

	if (xdiff > 0)
		angle += 2 * (180 - angle);

	this->sprite.setRotation(angle);

	this->animHandler.changeAnim(anims[currentAnim]);
	oldWalkState = walkState;
	walkState = WalkState::NONE;
}

void Player::update(float dt)
{
	float fX, fY;

	if (wayPoints.size() > 0)
	{
		sf::Vector2i to = wayPoints.top() - this->tilePos;
		sf::Vector2f dir = (sf::Vector2f)to;
		if (testTimer <= 0)//(to == sf::Vector2i{ 0, 0 })
		{
			setPos({ wayPoints.top().x * 32.f, wayPoints.top().y * 32.f });
			wayPoints.pop();
			//setPos({ tilePos.x * 32.f, tilePos.y * 32.f });
			testTimer = 0.25;
		}
		else
		{
			testTimer -= dt;
		}
		/*
		else
		{
			velocity = dir * speed;
			velocity = helper.clamp(velocity, maxSpeed);
			move(velocity * dt);
			setPos(position);
		}*/
	}
	/*totalForce += moveForce;

	fX = velocity.x == 0 ? 0 : friction;
	fY = velocity.y == 0 ? 0 : friction;

	acceleration.x = velocity.x > 0 ? (totalForce.x / mass) - fX
		: (totalForce.x / mass) + fX;
	acceleration.y = velocity.y > 0 ? (totalForce.y / mass) - fY
		: (totalForce.y / mass) + fY;

	velocity += acceleration;

	velocity.x = std::abs(velocity.x) <= std::abs(fX) ? 0 : velocity.x;
	velocity.y = std::abs(velocity.y) <= std::abs(fY) ? 0 : velocity.y;*/

	//velocity = helper.clamp(velocity, maxSpeed);

	//move(velocity * dt);


	//totalForce -= moveForce;
	//moveForce = { 0,0 };

	updateAbilities(dt);

	this->tilePos.x = std::ceil((position.x) / 32);
	this->tilePos.y = std::ceil((position.y) / 32);
	if (tilePos.x < 0)
		tilePos.x = 0;
	if (tilePos.y < 0)
		tilePos.y = 0;

	//setPos((sf::Vector2f)tilePos * 32.f);

	hud.update(dt);
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
