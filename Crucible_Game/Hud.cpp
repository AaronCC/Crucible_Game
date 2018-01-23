#include "Hud.h"

void Hud::draw(float dt)
{
	this->game->window.draw(elements["hGlobe"]);
	this->game->window.draw(elements["hPool"]);
}

void Hud::update(float dt)
{
}

void Hud::updateHealth(float percent)
{
	sf::IntRect hRect = elements["hPool"].getTextureRect();
	int newH = (1.f - percent)*(float)hRect.height;
	elements["hPool"].setTextureRect({ 0, newH,hRect.width,hRect.height });
	sf::Vector2f pos = elements["hGlobe"].getPosition();
	elements["hPool"].setPosition(pos.x, pos.y + newH);
}

Hud::~Hud()
{
}