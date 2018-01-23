#include "Helper.h"

sf::View Helper::resizeView(float windowW, float windowH, float aspectRatio)
{
	float windowRatio = windowW / (float)windowH;
	float viewRatio = aspectRatio;
	float sizeX = 1;
	float sizeY = 1;
	float posX = 0;
	float posY = 0;

	bool horizontalSpacing = true;
	if (windowRatio < viewRatio)
		horizontalSpacing = false;

	if (horizontalSpacing) {
		sizeX = viewRatio / windowRatio;
		posX = (1 - sizeX) / 2.f;
	}
	else {
		sizeY = windowRatio / viewRatio;
		posY = (1 - sizeY) / 2.f;
	}

	return sf::View(sf::FloatRect(posX, posY, sizeX, sizeY));
}

float Helper::magnitude(sf::Vector2f vec)
{
	return std::sqrt(std::pow(vec.x, 2) + std::pow(vec.y, 2));
}

sf::Vector2f Helper::normalized(sf::Vector2f vec, float mag)
{
	return { vec.x / mag, vec.y / mag };
}

sf::Vector2f Helper::clamp(sf::Vector2f vec, float max)
{
	float mag = magnitude(vec);
	if (mag > max)
	{
		sf::Vector2f norm = normalized(vec, mag);
		return norm * max;
	}
	else
		return vec;
}

Helper::Helper()
{
}


Helper::~Helper()
{
}
