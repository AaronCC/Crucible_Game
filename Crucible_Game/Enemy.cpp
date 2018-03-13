#include "Enemy.h"

void Enemy::resolveTick()
{
	std::vector<int> erase;
	int i = 0;
	for (auto eff : effs)
	{
		eff.dur -= 1;
		dealDamage(eff.damage.getDamage());
		if (eff.dur < 1)
		{
			this->stats = this->stats + eff.stats;
			erase.push_back(i);
		}
		i++;
	}
	i = 0;
	for (auto e : erase)
	{
		e -= i;
		effs.erase(effs.begin() + e);
		i++;
	}
}

void Enemy::dealDamage(int dmg)
{
	this->hp -= dmg;
	float p = hp / (float)maxHp;
	this->hpBar.setSize({ p*TILE_SIZE,4 });
	if (hp <= 0)
	{
		this->pos = { -1000.f,-1000.f };
		this->active = false;
		update();
	}
}

void Enemy::applyEff(AbEffect::Effect eff)
{
	eff.dur -= 1;
	dealDamage(eff.damage.getDamage());
	this->stats = this->stats - eff.stats;
	if (eff.dur > 0)
		effs.push_back(eff);
}

void Enemy::update()
{
	this->tilePos.x = std::ceil((pos.x) / (float)TILE_SIZE);
	this->tilePos.y = std::ceil((pos.y) / (float)TILE_SIZE);
	sf::Vector2f halfTSize = { TILE_SIZE / 2, TILE_SIZE / 2 };
	sprite.setPosition(pos);
	hpBar.setPosition(pos - halfTSize);
	hpBarBack.setPosition(pos - halfTSize);
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