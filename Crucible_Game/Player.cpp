#include "Player.h"

void Player::handleInput()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		this->animHandler.changeAnim(anims["W"]);
		moveForce.x -= speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		this->animHandler.changeAnim(anims["N"]);
		moveForce.y -= speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		this->animHandler.changeAnim(anims["S"]);
		moveForce.y += speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		this->animHandler.changeAnim(anims["E"]);
		moveForce.x += speed;
	}
	sf::Event e;
	while(this->game->window.pollEvent(e))
	switch (e.type)
	{
		case sf::Event::KeyPressed:
		{
			break;
		}
		
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

	return;
}

void Player::update(float dt)
{
	this->sprite.setPosition(position);
	float fX, fY;

	totalForce += moveForce;

	fX = velocity.x == 0 ? 0 : friction;
	fY = velocity.y == 0 ? 0 : friction;
	
	acceleration.x = velocity.x > 0 ? (totalForce.x / mass) - fX : (totalForce.x / mass) + fX; 
	acceleration.y = velocity.y > 0 ? (totalForce.y / mass) - fY : (totalForce.y / mass) + fY;
	
	velocity += acceleration;

	velocity.x = std::abs(velocity.x) <= std::abs(fX) ? 0 : velocity.x;
	velocity.y = std::abs(velocity.y) <= std::abs(fY) ? 0 : velocity.y;
	
	/*velocity.x = std::abs(velocity.x) >= maxSpeed ? maxSpeed * (velocity.x / velocity.x*-1) : velocity.x;
	velocity.y = std::abs(velocity.y) >= maxSpeed ? maxSpeed * (velocity.y / velocity.y*-1) : velocity.y;*/

	move(velocity);

	totalForce -= moveForce;
	moveForce = { 0,0 };
}

Player::~Player()
{
}
