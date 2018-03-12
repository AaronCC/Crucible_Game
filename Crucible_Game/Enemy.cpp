#include "Enemy.h"

void Enemy::dealDamage(int dmg)
{
	this->hp -= dmg;
	float p = hp / (float)maxHp;
	this->hpBar.setSize({ p*TILE_SIZE,4 });
}

void Enemy::update()
{
	sf::Vector2f halfTSize = { TILE_SIZE / 2, TILE_SIZE / 2 };
	hpBar.setPosition(pos-halfTSize);
	hpBarBack.setPosition(pos-halfTSize);
}

void Enemy::draw()
{
	this->game->window.draw(this->sprite);
	this->game->window.draw(this->hpBarBack);
	this->game->window.draw(this->hpBar);;
}

Enemy::~Enemy()
{
}